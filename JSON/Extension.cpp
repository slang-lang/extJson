
// Header
#include "Extension.h"

// Library includes

// Project includes
#include "FromJson.h"
#include "JsonEncode.h"
#include "ToJson.h"

// Namespace declarations


namespace Json {


Extension::Extension()
: Extensions::AExtension( "extJson", "0.2.3" )
{
    mName = "extJson (using libjsoncpp " + std::string( JSONCPP_VERSION_STRING ) + ")";
}

void Extension::initialize( Extensions::ExtensionNamespace* scope )
{
    (void) scope;
}

void Extension::provideMethods( Extensions::ExtensionMethods& methods )
{
    assert( methods.empty() );

    methods.push_back( new FromJson() );
    methods.push_back( new JsonEncode() );
    methods.push_back( new ToJson() );
}


}


extern "C" Extensions::AExtension* factory( void ) {
	return dynamic_cast<Extensions::AExtension*>( new Json::Extension() );
}
