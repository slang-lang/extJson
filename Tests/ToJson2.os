#!/usr/local/bin/slang

import System.Exception;
import System.IO.File;

public object TestObject {
	public string mName;
	public int mType;

	public void Constructor() {
		mName = "";
		mType = 0;
	}

	public void Constructor(string name, int type) {
		mName = name;
		mType = type;
	}
}

public object DerivedObject extends TestObject {
	public bool mFlag;

	public void Constructor() {
		base.Constructor();

		mFlag = false;
	}

	public void Constructor(string name, int type, bool flag) {
		base.Constructor(name, type);

		mFlag = flag;
	}
}

public object ExtendedObject extends DerivedObject {
	public float mFloat;
}

public void Main(int argc, string argv) {
	assert( TestCase1() );
	assert( TestCase2() );
	assert( TestCase3() );
	assert( TestCase4() );
}

private bool TestCase1() const {
	print("TestCase 1");

	Test_ToJson();
	Test_FromJson();

	print("");

	return true;
}

private bool TestCase2() const {
	print("TestCase 2");

	TestObject test = new TestObject("MeinName", 1731389);

	string json = ToJsonString(test);
	print("json = " + json);

	TestObject obj2 = new TestObject();
	FromJsonString(obj2, json);

	print("obj2={" + obj2.mName + "," + obj2.mType + "}");

	print("");

	return true;
}

private bool TestCase3() const {
	print("TestCase 3");

	Exception ex = new Exception("this is an exception");

	print(ToJsonString(ex));

	System.IO.File file = new System.IO.File();
	print(ToJsonString(file));

	print("");

	return true;
}

private bool TestCase4() const {
	print("TestCase 4");

	DerivedObject derived = new DerivedObject("derived", 173, true);

	string json = ToJsonString(derived);
	print("json = " + json);

	DerivedObject generated = new DerivedObject();
	FromJsonString(generated, json);

	print("generated={" + generated.mName + "," + generated.mType + "," + generated.mFlag + "}");

	print("");

	return true;
}

private bool Test_FromJson() const {
	TestObject obj = new TestObject();
	FromJsonString(obj, "{ \"mName\": \"test\" }");

	print("obj={" + obj.mName + "," + obj.mType + "}");

	return true;
}

private bool Test_ToJson() const {
	TestObject obj = new TestObject("test", 17);

	print("ToJson(obj) = " + ToJsonString(obj));

	return true;
}

