#!/usr/local/bin/slang

private object BaseObject {
	public int IntValue;
}

private object Derived extends BaseObject {
	public string StringValue;
}

private object Replica replicates Derived;

public void Main(int argc, string args) {
	assert( TestCase1() );
	assert( TestCase2() );
	//assert( TestCase3() );
}

private bool TestCase1() const {
	print( "TestCase 1" );

	try {
		var baseObject = new BaseObject();
		baseObject.IntValue = 173;

		string json = ToJson( baseObject );
		print( "baseObject = " + json );

		return true;
	}
	catch ( string e ) {
	    print( "Exception: " + e );
	}

	return false;
}

private bool TestCase2() const {
	print( "TestCase 2" );

	try {
		var derived = new Derived();
		print( "derived = " + ToJson( derived ) );

		string json = "{\"StringValue\":\"\",\"base\":{\"IntValue\":0}}";

		bool result = FromJson( derived, json );
		print( json + " == " + ToJson( derived ) );

		return result && json == ToJson( derived );
	}
	catch ( string e ) {
	    print( "Exception: " + e );
	}

	return false;
}

private bool TestCase3() const {
	print( "TestCase 3" );

	try {
		var replica = new Replica();

		string json = "{\"base\":{\"IntValue\":\"42\"},\"StringValue\":\"this is a string\"}";

		bool result = FromJson( replica, json );
		print( "result = " + result );

		print( json );
		print( ToJson( replica ) );

		result = result && json == ToJson( replica );

		print( "replica = " + ToJson( replica ) );

		return result;
	}
	catch ( string e ) {
	    print( "Exception: " + e );
	}

	return false;
}

