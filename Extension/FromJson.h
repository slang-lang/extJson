
#ifndef Slang_Extensions_System_FromJson_h
#define Slang_Extensions_System_FromJson_h


// Library includes
#include <json/reader.h>
#include <json/value.h>

// Project includes
#include <Core/Designtime/BuildInTypes/BoolType.h>
#include <Core/Designtime/BuildInTypes/StringType.h>
#include <Core/Extensions/ExtensionMethod.h>
#include <Core/Runtime/BuildInTypes/BoolType.h>
#include <Core/Runtime/BuildInTypes/StringType.h>
#include <Core/Runtime/Exceptions.h>
#include <Core/VirtualMachine/Controller.h>
#include "Defines.h"

// Forward declarations

// Namespace declarations


namespace Json {


class FromJson : public Extensions::ExtensionMethod
{
public:
	FromJson()
	: ExtensionMethod(0, "FromJson", Designtime::BoolType::TYPENAME)
	{
		ParameterList params;
		params.push_back(Parameter::CreateDesigntime("object", Common::TypeDeclaration(_any)));
		params.push_back(Parameter::CreateDesigntime("value", Common::TypeDeclaration(Designtime::StringType::TYPENAME)));

		setSignature(params);
	}

public:
	Runtime::ControlFlow::E execute( const ParameterList& params, Runtime::Object* result )
	{
		ParameterList list = mergeParameters(params);

		ParameterList::const_iterator it = list.begin();

		auto* param_object = Controller::Instance().memory()->get((*it++).reference());
		if ( !param_object ) {
			throw Runtime::Exceptions::AccessViolation("invalid reference set for 'object'" );
		}

		auto param_string = (*it++).value().toStdString();

		Json::Value root;
		Json::Reader reader;
		reader.parse( param_string, root );

		bool success = fromJson( root, param_object );

		*result = Runtime::BoolType( success );

		return Runtime::ControlFlow::Normal;
	}

private:
	bool fromJson( const Json::Value& value, Runtime::Object* result ) const {
		for ( const auto& name : value.getMemberNames() ) {
			const Json::Value& sub = value[ name ];

			auto* symbol = result->resolve( name, false, Visibility::Designtime );
			if ( !symbol ) {
                continue;   // we don't care for missing members
			}

			auto *obj = dynamic_cast<Runtime::Object*>( symbol );
			if ( !obj ) {
                // TODO: why would this happen?
				continue;
			}

			if ( obj->isAtomicType() ) {
                if ( sub.isObject() || sub.isArray() ) {
                    throw "not atomic: '" + name + "'";
                }

                switch ( obj->getValue().type() ) {
                    case Slang::Runtime::AtomicValue::Type::BOOL:   obj->setValue( sub.asBool() ); break;
                    case Slang::Runtime::AtomicValue::Type::DOUBLE: obj->setValue( sub.asDouble() ); break;
                    case Slang::Runtime::AtomicValue::Type::FLOAT:  obj->setValue( sub.asFloat() ); break;
                    case Slang::Runtime::AtomicValue::Type::INT:    obj->setValue( sub.asInt() ); break;
                    case Slang::Runtime::AtomicValue::Type::STRING: obj->setValue( sub.asString() ); break;
                    default: throw "invalid_type";
                }
			}
			else {
                // TODO: we need to check for null pointers

				fromJson( sub, obj );
			}
		}

		return true;
	}
};


}


#endif
