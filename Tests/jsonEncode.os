#!/usr/local/bin/slang

import libJson;

public void Main(int argc, string args) {
	assert( TestCase1() );
}


private bool TestCase1() const {
	print( "TestCase 1" );

	try {
		write( "Wait for keypress" ); cin();

		var value = jsonEncode( "{ \"key\": \"value\" }" );
		assert( value );
		print( typeid( value ) );
		print( value.toString() );
	}
	catch ( string e ) {
	    print( "Exception: " + e );
	}

	return false;
}

