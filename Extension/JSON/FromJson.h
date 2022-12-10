
#ifndef Slang_Extensions_System_FromJson_h
#define Slang_Extensions_System_FromJson_h


// Library includes
#include <json/reader.h>
#include <json/value.h>

// Project includes
#include <Core/Designtime/BuildInTypes/BoolObject.h>
#include <Core/Designtime/BuildInTypes/StringObject.h>
#include <Core/Extensions/ExtensionMethod.h>
#include <Core/Runtime/BuildInTypes/BoolObject.h>
#include <Core/Runtime/BuildInTypes/StringObject.h>
#include <Core/Runtime/Exceptions.h>
#include <Core/Tools.h>
#include <Core/VirtualMachine/Controller.h>

// Forward declarations

// Namespace declarations
using namespace Slang;


namespace Json {


class FromJson : public Slang::Extensions::ExtensionMethod
{
public:
	FromJson()
	: ExtensionMethod(0, "FromJson", Designtime::BoolObject::TYPENAME)
	{
		ParameterList params;
		params.push_back(Parameter::CreateDesigntime("object", Common::TypeDeclaration(_any)));
		params.push_back(Parameter::CreateDesigntime("value", Common::TypeDeclaration(Designtime::StringObject::TYPENAME)));

		setSignature(params);
	}

public:
	Runtime::ControlFlow::E execute(Common::ThreadId threadId, const ParameterList& params, Runtime::Object* result, const Token& token)
	{
		ParameterList list = mergeParameters(params);

		try {
			ParameterList::const_iterator it = list.begin();

			Runtime::Object* param_object = Controller::Instance().memory()->get((*it++).reference());
			if ( !param_object ) {
				throw Runtime::Exceptions::AccessViolation("invalid reference set for 'object'", token.position());
			}

            auto param_string = (*it++).value().toStdString();

            Json::Value root;
            Json::Reader reader;
            reader.parse( param_string, root );

			bool success = fromJson( root, param_object );

			*result = Runtime::BoolObject( success );
		}
		catch ( std::exception &e ) {
			Runtime::Object *data = Controller::Instance().repository()->createInstance(Runtime::StringObject::TYPENAME, ANONYMOUS_OBJECT);
			*data = Runtime::StringObject(std::string(e.what()));

			Controller::Instance().thread(threadId)->exception() = Runtime::ExceptionData(data, token.position());
			return Runtime::ControlFlow::Throw;
		}

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
