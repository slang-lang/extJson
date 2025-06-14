
#ifndef Slang_Extensions_System_JsonEncode_h
#define Slang_Extensions_System_JsonEncode_h


// Library includes
#include <json/reader.h>
#include <json/value.h>

// Project includes
#include <Core/Designtime/BuildInTypes/StringType.h>
#include <Core/Extensions/ExtensionMethod.h>
#include <Core/Runtime/BuildInTypes/StringType.h>
#include <Core/Runtime/Exceptions.h>
#include <Core/Tools.h>
#include <Core/VirtualMachine/Controller.h>
#include "Defines.h"

// Forward declarations

// Namespace declarations


namespace Json {


class JsonEncode : public Extensions::ExtensionMethod
{
public:
    JsonEncode()
	: ExtensionMethod(0, "jsonEncode", "JsonValue")
	{
		ParameterList params;
		params.push_back(Parameter::CreateDesigntime("source", Designtime::StringType::TYPENAME));

		setSignature(params);
	}

public:
	Runtime::ControlFlow::E execute( const ParameterList& params, Runtime::Object* result )
	{
		ParameterList list = mergeParameters(params);

        ParameterList::const_iterator it = list.begin();

        auto param_string = (*it++).value().toStdString();

        Json::Value root;
        Json::Reader reader;
        reader.parse( param_string, root );

        *result = *Controller::Instance().repository()->createReference("JsonObject", ANONYMOUS_OBJECT, PrototypeConstraints(), Repository::InitilizationType::Final );

        parse( root, result );

		return Runtime::ControlFlow::Normal;
	}

private:
    void parse( const Json::Value& value, Runtime::Object* result ) const {
        for ( const auto& name : value.getMemberNames() ) {
            const Json::Value& sub = value[ name ];

            if ( sub.isArray() ) {
                auto* jsonArray = Controller::Instance().repository()->createReference("JsonArray", ANONYMOUS_OBJECT, PrototypeConstraints(), Repository::InitilizationType::Final );

                //parseArray( sub, jsonArray );

                result->defineMember( name, jsonArray );
            }
            else if ( sub.isObject() ) {
                auto* jsonObject = Controller::Instance().repository()->createReference("JsonObject", ANONYMOUS_OBJECT, PrototypeConstraints(), Repository::InitilizationType::Final );

                parseObject( sub, jsonObject );

                result->defineMember( name, jsonObject );
            }
            else {
                auto* jsonValue = Controller::Instance().repository()->createReference("JsonValue", ANONYMOUS_OBJECT, PrototypeConstraints(), Repository::InitilizationType::Final );

                {   // mValue
                    auto* mValueSymbol = jsonValue->resolve( "mValue", false, Visibility::Designtime );
                    if ( !mValueSymbol ) {
                        throw Runtime::Exceptions::InvalidSymbol( "mValue" );
                    }

                    auto *mValueObject = dynamic_cast<Runtime::Object*>( mValueSymbol );
                    mValueObject->setValue( sub.asString() );
                }
                {   // mValueType
                    auto* mValueTypeSymbol = jsonValue->resolve( "mValue", false, Visibility::Designtime );
                    if ( !mValueTypeSymbol ) {
                        throw Runtime::Exceptions::InvalidSymbol( "mValueType" );
                    }

                    auto *mValueTypeObject = dynamic_cast<Runtime::Object*>( mValueTypeSymbol );
                    mValueTypeObject->setValue( 5 );
                }
            }
        }
    }

    void parseObject( const Json::Value& value, Runtime::Object* result ) const {
        for ( const auto& name : value.getMemberNames() ) {
            const Json::Value& sub = value[ name ];

            if ( sub.isArray() ) {
                auto* jsonArray = Controller::Instance().repository()->createInstance("JsonArray", ANONYMOUS_OBJECT);

                //parseArray( sub, jsonArray );

                result->defineMember( name, jsonArray );
            }
            else if ( sub.isObject() ) {
                auto* jsonObject = Controller::Instance().repository()->createInstance("JsonObject", ANONYMOUS_OBJECT);

                parseObject( sub, jsonObject );

                result->defineMember( name, jsonObject );
            }
            else {
                auto* jsonValue = Controller::Instance().repository()->createInstance("JsonValue", ANONYMOUS_OBJECT);

                parse( sub, jsonValue );

                result->defineMember( name, jsonValue );
            }
        }
    }
};


}


#endif
