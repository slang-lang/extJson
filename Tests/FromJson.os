#!/usr/local/bin/slang

private object BaseObject {
	public int IntValue;
}

private object DerivedObject extends BaseObject {
	public string StringValue;
}

public void Main( int argc, string argv ) {
	assert( TestCase1() );
	assert( TestCase2() );
}

private bool TestCase1() const {
	print( "TestCase 1" );

    try {
        BaseObject baseObject = new BaseObject();
        baseObject.IntValue = 173;

        string json = "{\"IntValue\":173}";
/*
        string json = ToJsonString( baseObject );
        print( "json = " + json );
*/

        bool result = FromJsonString( baseObject, json );
        return result && baseObject.IntValue == 173;
	}
	catch ( string e ) {
	    print( "Exception: " + e );
	}

	return false;
}

private bool TestCase2() const {
	print( "TestCase 2" );

    try {
        DerivedObject derived = new DerivedObject();
        derived.IntValue = 42;
        derived.StringValue = "this is a string";

        string json = ToJsonString( derived );
        print( "json = " + json );

        bool result = FromJsonString( derived, json );
        return result && derived.IntValue == 42;
	}
	catch ( string e ) {
	    print( "Exception: " + e );
	}

	return false;
}

