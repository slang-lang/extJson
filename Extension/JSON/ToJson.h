
#ifndef Slang_Extensions_System_ToJson_h
#define Slang_Extensions_System_ToJson_h


// Library includes
#include <json/value.h>
#include <json/writer.h>

// Project includes
#include <Core/Designtime/BuildInTypes/StringType.h>
#include <Core/Extensions/ExtensionMethod.h>
#include <Core/Runtime/BuildInTypes/StringType.h>
#include <Core/Runtime/Exceptions.h>
#include <Core/Tools.h>
#include <Core/VirtualMachine/Controller.h>

// Forward declarations

// Namespace declarations
using namespace Slang;


namespace Json {


class ToJson : public Slang::Extensions::ExtensionMethod
{
public:
	ToJson()
	: ExtensionMethod(0, "ToJson", Designtime::StringType::TYPENAME)
	{
		ParameterList params;
		params.push_back(Parameter::CreateDesigntime("source", Common::TypeDeclaration(VALUE_NONE)));

		setSignature(params);
	}

public:
	Runtime::ControlFlow::E execute(Common::ThreadId threadId, const ParameterList& params, Runtime::Object* result, const Token& token)
	{
		ParameterList list = mergeParameters(params);

		try {
			ParameterList::const_iterator it = list.begin();

			Runtime::Object *param_object = Controller::Instance().memory()->get((*it++).reference());
			if ( !param_object ) {
				throw Runtime::Exceptions::AccessViolation("invalid reference set for 'object'", token.position());
			}

			Json::Value value;

			toJson( param_object, value );

			Json::FastWriter writer;

			*result = Runtime::StringType( writer.write( value ) );
		}
		catch ( std::exception &e ) {
			Runtime::Object *data = Controller::Instance().repository()->createInstance(Runtime::StringType::TYPENAME, ANONYMOUS_OBJECT);
			*data = Runtime::StringType(std::string(e.what()));

			Controller::Instance().thread(threadId)->exception() = Runtime::ExceptionData(data, token.position());
			return Runtime::ControlFlow::Throw;
		}

		return Runtime::ControlFlow::Normal;
	}

private:
	void toJson( Runtime::Object* object, Json::Value& result ) const {
		for ( auto it = object->beginSymbols(); it != object->endSymbols(); ++it ) {
			if ( it->first == IDENTIFIER_THIS || !it->second || it->second->getSymbolType() != Symbol::IType::ObjectSymbol ) {
				continue;
			}

			if ( it->first == IDENTIFIER_BASE ) {
				auto* obj = dynamic_cast<Runtime::Object*>( it->second );
				if ( !obj || obj->QualifiedTypename() == _object ) {
					// TODO: why would this happen?
					continue;
				}

				Json::Value value;
				toJson( obj, value );

				result[ it->first ] = value;
				continue;
			}

			auto *obj = dynamic_cast<Runtime::Object*>( it->second );
			if ( !obj ) {
				// TODO: why would this happen?
				continue;
			}

			switch ( obj->getValue().type() ) {
				case Slang::Runtime::AtomicValue::Type::BOOL:   result[ it->first ] = obj->getValue().toBool(); break;
				case Slang::Runtime::AtomicValue::Type::DOUBLE: result[ it->first ] = obj->getValue().toDouble(); break;
				case Slang::Runtime::AtomicValue::Type::FLOAT:  result[ it->first ] = obj->getValue().toFloat(); break;
				case Slang::Runtime::AtomicValue::Type::INT:    result[ it->first ] = static_cast<Int64>( obj->getValue().toInt() ); break;
				case Slang::Runtime::AtomicValue::Type::STRING: result[ it->first ] = obj->getValue().toStdString(); break;
				default:                                        result[ it->first ] = obj->getValue().toStdString(); break;
			}
		}
	}
};


}


#endif
