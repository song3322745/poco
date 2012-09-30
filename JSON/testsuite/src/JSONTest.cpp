//
// JSONTest.cpp
//
// $Id: //poco/1.4/XML/testsuite/src/JSONTest.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "JSONTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"

#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/JSON/DefaultHandler.h"
#include "Poco/JSON/Template.h"

#include "Poco/Path.h"
#include "Poco/Environment.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/Glob.h"

#include <set>

using namespace Poco::JSON;
using namespace Poco::Dynamic;


JSONTest::JSONTest(const std::string& name): CppUnit::TestCase("JSON")
{

}


JSONTest::~JSONTest()
{

}


void JSONTest::setUp()
{
}


void JSONTest::tearDown()
{
}


void JSONTest::testNullProperty()
{
	std::string json = "{ \"test\" : null }";
	Parser parser;

	Var result;
	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}
	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	assert(object->isNull("test"));
	Var test = object->get("test");
	assert(test.isEmpty());
}


void JSONTest::testTrueProperty()
{
	std::string json = "{ \"test\" : true }";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.type() == typeid(bool));
	bool value = test;
	assert(value);
}


void JSONTest::testFalseProperty()
{
	std::string json = "{ \"test\" : false }";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.type() == typeid(bool));
	bool value = test;
	assert(!value);
}


void JSONTest::testNumberProperty()
{
	std::string json = "{ \"test\" : 1969 }";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isInteger());
	int value = test;
	assert(value == 1969);
}


void JSONTest::testStringProperty()
{
	std::string json = "{ \"test\" : \"value\" }";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isString());
	std::string value = test;
	assert(value.compare("value") == 0);
}


void JSONTest::testEmptyObject()
{
	std::string json = "{}";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	assert(object->size() == 0);
}


void JSONTest::testDoubleProperty()
{
	std::string json = "{ \"test\" : 123.45 }";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isNumeric());
	double value = test;
	assert(value == 123.45);
}


void JSONTest::testDouble2Property()
{
	std::string json = "{ \"test\" : 12e34 }";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isNumeric());
	double value = test;
	assert(value >= 1.99e34 && value <= 12.01e34);
}


void JSONTest::testDouble3Property()
{
	std::string json = "{ \"test\" : 12e-34 }";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isNumeric());
	double value = test;
	assert(value == 12e-34);
}


void JSONTest::testObjectProperty()
{
	std::string json = "{ \"test\" : { \"property\" : \"value\" } }";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.type() == typeid(Object::Ptr));
	object = test.extract<Object::Ptr>();

	test = object->get("property");
	assert(test.isString());
	std::string value = test;
	assert(value.compare("value") == 0);
}


void JSONTest::testEmptyArray()
{
	std::string json = "[]";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	assert(array->size() == 0);
}


void JSONTest::testNestedArray()
{
	std::string json = "[[[[]]]]";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	assert(array->size() == 1);
}


void JSONTest::testNullElement()
{
	std::string json = "[ null ]";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	assert(array->isNull(0));
	Var test = array->get(0);
	assert(test.isEmpty());
}


void JSONTest::testTrueElement()
{
	std::string json = "[ true ]";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	Var test = array->get(0);
	assert(test.type() == typeid(bool));
	bool value = test;
	assert(value);
}


void JSONTest::testFalseElement()
{
	std::string json = "[ false ]";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	Var test = array->get(0);
	assert(test.type() == typeid(bool));
	bool value = test;
	assert(!value);
}


void JSONTest::testNumberElement()
{
	std::string json = "[ 1969 ]";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	Var test = array->get(0);
	assert(test.isInteger());
	int value = test;
	assert(value == 1969);
}


void JSONTest::testStringElement()
{
	std::string json = "[ \"value\" ]";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	Var test = array->get(0);
	assert(test.isString());
	std::string value = test;
	assert(value.compare("value") == 0);
}


void JSONTest::testEmptyObjectElement()
{
	std::string json = "[{}]";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	Object::Ptr object = array->getObject(0);
	assert(object->size() == 0);
}


void JSONTest::testDoubleElement()
{
	std::string json = "[ 123.45 ]";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	Var test = array->get(0);
	assert(test.isNumeric());
	double value = test;
	assert(value == 123.45);
}


void JSONTest::testOptValue()
{
	std::string json = "{ }";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	int n = object->optValue("test", 123);
	assert(n == 123);
}


void JSONTest::testQuery()
{
	std::string json = "{ \"name\" : \"Franky\", \"children\" : [ \"Jonas\", \"Ellen\" ] }";
	Parser parser;
	Var result;

	try
	{
		DefaultHandler handler;
		parser.setHandler(&handler);
		parser.parse(json);
		result = handler.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Query query(result);

	std::string firstChild = query.findValue("children[0]", "");
	assert(firstChild.compare("Jonas") == 0);
}


void JSONTest::testValidJanssonFiles()
{
	Poco::Path pathPattern(getTestFilesPath("valid"));

	std::set<std::string> paths;
	Poco::Glob::glob(pathPattern, paths);

	for(std::set<std::string>::iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Poco::Path filePath(*it, "input");

		if ( filePath.isFile() )
		{
			Poco::File inputFile(filePath);
			if ( inputFile.exists() )
			{
				Poco::FileInputStream fis(filePath.toString());
				std::cout << filePath.toString() << std::endl;

				Parser parser;
				Var result;

				try
				{
					DefaultHandler handler;
					parser.setHandler(&handler);
					parser.parse(fis);
					result = handler.result();
					std::cout << "Ok!" << std::endl;
				}
				catch(JSONException& jsone)
				{
					std::string err = jsone.displayText();
					std::cout << "Failed:" << err << std::endl;
					fail (err);
				}
				catch(Poco::Exception& e)
				{
					std::string err = e.displayText();
					std::cout << "Failed:" << err << std::endl;
					fail (err);
				}
			}
		}
	}
}


void JSONTest::testInvalidJanssonFiles()
{
	Poco::Path pathPattern(getTestFilesPath("invalid"));

	std::set<std::string> paths;
	Poco::Glob::glob(pathPattern, paths);

	for(std::set<std::string>::iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Poco::Path filePath(*it, "input");

		if ( filePath.isFile() )
		{
			Poco::File inputFile(filePath);
			if ( inputFile.exists() )
			{
				Poco::FileInputStream fis(filePath.toString());
				std::cout << filePath.toString() << std::endl;

				Parser parser;
				Var result;

				try
				{
					DefaultHandler handler;
					parser.setHandler(&handler);
					parser.parse(fis);
					result = handler.result();
					// We shouldn't get here.
					std::cout << "We didn't get an exception. This is the result: " << result.convert<std::string>() << std::endl; 
					fail(result.convert<std::string>());
				}
				catch(JSONException&)
				{
					continue;
				}
				catch(Poco::SyntaxException&)
				{ }
			}
		}
	}
}


void JSONTest::testTemplate()
{
	Template tpl;
	tpl.parse("Hello world! From <?= person.name ?>\n<?if person.toOld ?>You're too old<?endif?>\n");

	Object::Ptr data = new Object();
	Object::Ptr person = new Object();
	data->set("person", person);
	person->set("name", "Franky");
	person->set("toOld", true);
	tpl.render(data, std::cout);
}


std::string JSONTest::getTestFilesPath(const std::string& type)
{
	std::ostringstream ostr;
	ostr << "data/" << type << '/';
	std::string validDir(ostr.str());
	Poco::Path pathPattern(validDir);
	if (Poco::File(pathPattern).exists())
	{
		validDir += '*';
		return validDir;
	}

	ostr.str("");
	ostr << "/JSON/testsuite/data/" << type << '/';
	validDir = Poco::Environment::get("POCO_BASE") + ostr.str();
	pathPattern = validDir;

	if (Poco::File(pathPattern).exists())
		validDir += '*';
	else
		throw Poco::NotFoundException("cannot locate directory containing valid JSON test files");

	return validDir;
}


CppUnit::Test* JSONTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("JSONTest");

	CppUnit_addTest(pSuite, JSONTest, testNullProperty);
	CppUnit_addTest(pSuite, JSONTest, testTrueProperty);
	CppUnit_addTest(pSuite, JSONTest, testFalseProperty);
	CppUnit_addTest(pSuite, JSONTest, testNumberProperty);
	CppUnit_addTest(pSuite, JSONTest, testStringProperty);
	CppUnit_addTest(pSuite, JSONTest, testEmptyObject);
	CppUnit_addTest(pSuite, JSONTest, testDoubleProperty);
	CppUnit_addTest(pSuite, JSONTest, testDouble2Property);
	CppUnit_addTest(pSuite, JSONTest, testDouble3Property);
	CppUnit_addTest(pSuite, JSONTest, testObjectProperty);
	CppUnit_addTest(pSuite, JSONTest, testEmptyArray);
	CppUnit_addTest(pSuite, JSONTest, testNestedArray);
	CppUnit_addTest(pSuite, JSONTest, testNullElement);
	CppUnit_addTest(pSuite, JSONTest, testTrueElement);
	CppUnit_addTest(pSuite, JSONTest, testFalseElement);
	CppUnit_addTest(pSuite, JSONTest, testNumberElement);
	CppUnit_addTest(pSuite, JSONTest, testStringElement);
	CppUnit_addTest(pSuite, JSONTest, testEmptyObjectElement);
	CppUnit_addTest(pSuite, JSONTest, testDoubleElement);
	CppUnit_addTest(pSuite, JSONTest, testOptValue);
	CppUnit_addTest(pSuite, JSONTest, testQuery);
	CppUnit_addTest(pSuite, JSONTest, testValidJanssonFiles);
	CppUnit_addTest(pSuite, JSONTest, testInvalidJanssonFiles);
	CppUnit_addTest(pSuite, JSONTest, testTemplate);

	return pSuite;
}
