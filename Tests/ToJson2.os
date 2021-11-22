#!/usr/local/bin/slang

import System.Exception;
import System.IO.File;

public object TestObject {
	public string Name;
	public int Type;

	public void Constructor() {
		Name = "";
		Type = 0;
	}

	public void Constructor( string name, int type ) {
		Name = name;
		Type = type;
	}
}

public object DerivedObject extends TestObject {
	public bool Flag;

	public void Constructor() {
		base.Constructor();

		Flag = false;
	}

	public void Constructor( string name, int type, bool flag ) {
		base.Constructor( name, type );

		Flag = flag;
	}
}

public object ExtendedObject extends DerivedObject {
	public float Float;
}

public void Main(int argc, string argv) {
	assert( TestCase1() );
	assert( TestCase2() );
	assert( TestCase3() );
	assert( TestCase4() );
}

private bool TestCase1() const {
	print( "TestCase 1" );

	Test_ToJson();
	Test_FromJson();

	print( "" );

	return true;
}

private bool TestCase2() const {
	print( "TestCase 2" );

	var test = new TestObject( "MeinName", 1731389 );

	string json = ToJson( test );
	print( "json = " + json );

	var obj2 = new TestObject();
	FromJson( obj2, json );

	print( "obj2={" + obj2.Name + "," + obj2.Type + "}" );

	print( "" );

	return true;
}

private bool TestCase3() const {
	print( "TestCase 3" );

	var ex = new Exception( "this is an exception" );
	print( ToJson( ex ) );

	var file = new System.IO.File();
	print( ToJson( file ) );

	print( "" );

	return true;
}

private bool TestCase4() const {
	print( "TestCase 4" );

	var derived = new DerivedObject( "derived", 173, true );

	string json = ToJson( derived );
	print( "json = " + json );

	var generated = new DerivedObject();
	FromJson( generated, json );

	print( "generated={" + generated.Name + "," + generated.Type + "," + generated.Flag + "}" );

	print( "" );

	return true;
}

private bool Test_FromJson() const {
	var obj = new TestObject();

	FromJson( obj, "{ \"Name\": \"test\" }" );

	print( "obj={" + obj.Name + "," + obj.Type + "}" );

	return true;
}

private bool Test_ToJson() const {
	var obj = new TestObject( "test", 17 );

	print( "ToJson(obj) = " + ToJson( obj ) );

	return true;
}

