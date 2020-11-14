/*
 * test_inifile.cpp
 *
 * Created on: 26 Dec 2015
 *     Author: Fabian Meyer
 *    License: MIT
 */

#include "inicpp.h"
#include <catch.hpp>
#include <cstring>
#include <sstream>

TEST_CASE("parse ini file", "IniFile")
{
    std::istringstream ss(("[Foo]\nbar=hello world\n[Test]"));
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 2);
    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "hello world");
    REQUIRE(inif["Test"].size() == 0);
}

TEST_CASE("parse empty file", "IniFile")
{
    std::istringstream ss("");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 0);
}

TEST_CASE("parse comment only file", "IniFile")
{
    std::istringstream ss("# this is a comment");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 0);
}


TEST_CASE("parse empty section", "IniFile")
{
    std::istringstream ss("[Foo]");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 0);
}

TEST_CASE("parse empty field", "IniFile")
{
    std::istringstream ss("[Foo]\nbar=");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 1);
    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "");
}

TEST_CASE("parse section with duplicate field", "IniFile")
{
    std::istringstream ss("[Foo]\nbar=hello\nbar=world");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 1);
    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "world");
}

TEST_CASE("parse field as bool", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=true\nbar2=false\nbar3=tRuE");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 3);
    REQUIRE(inif["Foo"]["bar1"].as<bool>());
    REQUIRE_FALSE(inif["Foo"]["bar2"].as<bool>());
    REQUIRE(inif["Foo"]["bar3"].as<bool>());
}

TEST_CASE("parse field as char", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=c\nbar2=q");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 2);
    REQUIRE(inif["Foo"]["bar1"].as<char>() == 'c');
    REQUIRE(inif["Foo"]["bar2"].as<char>() == 'q');
}

TEST_CASE("parse field as unsigned char", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=c\nbar2=q");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 2);
    REQUIRE(inif["Foo"]["bar1"].as<unsigned char>() == 'c');
    REQUIRE(inif["Foo"]["bar2"].as<unsigned char>() == 'q');
}

TEST_CASE("parse field as short", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=1\nbar2=-2");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 2);
    REQUIRE(inif["Foo"]["bar1"].as<short>() == 1);
    REQUIRE(inif["Foo"]["bar2"].as<short>() == -2);
}

TEST_CASE("parse field as unsigned short", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=1\nbar2=13");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 2);
    REQUIRE(inif["Foo"]["bar1"].as<unsigned short>() == 1);
    REQUIRE(inif["Foo"]["bar2"].as<unsigned short>() == 13);
}

TEST_CASE("parse field as int", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=1\nbar2=-2");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 2);
    REQUIRE(inif["Foo"]["bar1"].as<int>() == 1);
    REQUIRE(inif["Foo"]["bar2"].as<int>() == -2);
}

TEST_CASE("parse field as unsigned int", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=1\nbar2=13");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 2);
    REQUIRE(inif["Foo"]["bar1"].as<unsigned int>() == 1);
    REQUIRE(inif["Foo"]["bar2"].as<unsigned int>() == 13);
}

TEST_CASE("parse field as long", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=1\nbar2=-2");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 2);
    REQUIRE(inif["Foo"]["bar1"].as<long>() == 1);
    REQUIRE(inif["Foo"]["bar2"].as<long>() == -2);
}

TEST_CASE("parse field as unsigned long", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=1\nbar2=13");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 2);
    REQUIRE(inif["Foo"]["bar1"].as<unsigned long>() == 1);
    REQUIRE(inif["Foo"]["bar2"].as<unsigned long>() == 13);
}

TEST_CASE("parse field as double", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=1.2\nbar2=1\nbar3=-2.4");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 3);
    REQUIRE(inif["Foo"]["bar1"].as<double>() == Approx(1.2).margin(1e-3));
    REQUIRE(inif["Foo"]["bar2"].as<double>() == Approx(1.0).margin(1e-3));
    REQUIRE(inif["Foo"]["bar3"].as<double>() == Approx(-2.4).margin(1e-3));
}

TEST_CASE("parse field as float", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=1.2\nbar2=1\nbar3=-2.4");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 3);
    REQUIRE(inif["Foo"]["bar1"].as<float>() == Approx(1.2f).margin(1e-3f));
    REQUIRE(inif["Foo"]["bar2"].as<float>() == Approx(1.0f).margin(1e-3f));
    REQUIRE(inif["Foo"]["bar3"].as<float>() == Approx(-2.4f).margin(1e-3f));
}

TEST_CASE("parse field as std::string", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=hello\nbar2=world");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 2);
    REQUIRE(inif["Foo"]["bar1"].as<std::string>() == "hello");
    REQUIRE(inif["Foo"]["bar2"].as<std::string>() == "world");
}

TEST_CASE("parse field as const char*", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1=hello\nbar2=world");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 2);
    REQUIRE(std::strcmp(inif["Foo"]["bar1"].as<const char*>(), "hello") == 0);
    REQUIRE(std::strcmp(inif["Foo"]["bar2"].as<const char*>(), "world") == 0);
}

TEST_CASE("parse field with custom field sep", "IniFile")
{
    std::istringstream ss("[Foo]\nbar1:true\nbar2:false\nbar3:tRuE");
    ini::IniFile inif(ss, ':');

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 3);
    REQUIRE(inif["Foo"]["bar1"].as<bool>());
    REQUIRE_FALSE(inif["Foo"]["bar2"].as<bool>());
    REQUIRE(inif["Foo"]["bar3"].as<bool>());
}

TEST_CASE("parse with comment", "IniFile")
{
    std::istringstream ss("[Foo]\n# this is a test\nbar=bla");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 1);
    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "bla");
}

TEST_CASE("parse with custom comment char prefix", "IniFile")
{
    std::istringstream ss("[Foo]\n$ this is a test\nbar=bla");
    ini::IniFile inif(ss, '=', '$');

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 1);
    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "bla");
}

TEST_CASE("parse with multi char comment prefix", "IniFile")
{
    std::istringstream ss("[Foo]\nREM this is a test\nbar=bla");
    ini::IniFile inif(ss, '=', {"REM"});

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 1);
    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "bla");
}

TEST_CASE("parse with multiple multi char comment prefixes", "IniFile")
{
    std::istringstream ss("[Foo]\n"
                          "REM this is a comment\n"
                          "#Also a comment\n"
                          "//Even this is a comment\n"
                          "bar=bla");
    ini::IniFile inif(ss, '=', {"REM", "#", "//"});

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 1);
    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "bla");
}

TEST_CASE("comment prefixes can be set after construction", "IniFile")
{
    std::istringstream ss("[Foo]\n"
                          "REM this is a comment\n"
                          "#Also a comment\n"
                          "//Even this is a comment\n"
                          "bar=bla");
    ini::IniFile inif;
    inif.setCommentPrefixes({"REM", "#", "//"});
    inif.decode(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 1);
    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "bla");
}

TEST_CASE("comments are allowed after escaped comments", "IniFile")
{
    std::istringstream ss("[Foo]\n"
                          "hello=world \\## this is a comment\n"
                          "more=of this \\# \\#\n");
    ini::IniFile inif(ss);

    REQUIRE(inif["Foo"]["hello"].as<std::string>() == "world #");
    REQUIRE(inif["Foo"]["more"].as<std::string>() == "of this # #");
}

TEST_CASE(
    "escape char right before a comment prefix escapes all the comment prefix",
    "IniFile")
{
    std::istringstream ss("[Foo]\n"
                          "weird1=note \\### this is not a comment\n"
                          "weird2=but \\#### this is a comment");
    ini::IniFile inif(ss, '=', {"##"});

    REQUIRE(inif["Foo"]["weird1"].as<std::string>() ==
            "note ### this is not a comment");
    REQUIRE(inif["Foo"]["weird2"].as<std::string>() == "but ##");
}

TEST_CASE("save with bool fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = true;
    inif["Foo"]["bar2"] = false;

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=true\nbar2=false\n");
}

TEST_CASE("save with char fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = static_cast<char>('c');
    inif["Foo"]["bar2"] = static_cast<char>('q');

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=c\nbar2=q\n");
}

TEST_CASE("save with unsigned char fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = static_cast<unsigned char>('c');
    inif["Foo"]["bar2"] = static_cast<unsigned char>('q');

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=c\nbar2=q\n");
}

TEST_CASE("save with short fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = static_cast<short>(1);
    inif["Foo"]["bar2"] = static_cast<short>(-2);

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=1\nbar2=-2\n");
}

TEST_CASE("save with unsigned short fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = static_cast<unsigned short>(1);
    inif["Foo"]["bar2"] = static_cast<unsigned short>(13);

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=1\nbar2=13\n");
}

TEST_CASE("save with int fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = static_cast<int>(1);
    inif["Foo"]["bar2"] = static_cast<int>(-2);

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=1\nbar2=-2\n");
}

TEST_CASE("save with unsigned int fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = static_cast<unsigned int>(1);
    inif["Foo"]["bar2"] = static_cast<unsigned int>(13);

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=1\nbar2=13\n");
}

TEST_CASE("save with long fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = static_cast<long>(1);
    inif["Foo"]["bar2"] = static_cast<long>(-2);

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=1\nbar2=-2\n");
}

TEST_CASE("save with unsigned long fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = static_cast<unsigned long>(1);
    inif["Foo"]["bar2"] = static_cast<unsigned long>(13);

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=1\nbar2=13\n");
}

TEST_CASE("save with double fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = static_cast<double>(1.2);
    inif["Foo"]["bar2"] = static_cast<double>(-2.4);

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=1.2\nbar2=-2.4\n");
}

TEST_CASE("save with float fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = static_cast<float>(1.2f);
    inif["Foo"]["bar2"] = static_cast<float>(-2.4f);

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=1.2\nbar2=-2.4\n");
}


TEST_CASE("save with std::string fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = static_cast<std::string>("hello");
    inif["Foo"]["bar2"] = static_cast<std::string>("world");

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=hello\nbar2=world\n");
}

TEST_CASE("save with const char* fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = static_cast<const char*>("hello");
    inif["Foo"]["bar2"] = static_cast<const char*>("world");

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=hello\nbar2=world\n");
}

TEST_CASE("save with char* fields", "IniFile")
{
    ini::IniFile inif;
    char bar1[6] = "hello";
    char bar2[6] = "world";
    inif["Foo"]["bar1"] = static_cast<char*>(bar1);
    inif["Foo"]["bar2"] = static_cast<char*>(bar2);

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=hello\nbar2=world\n");
}

TEST_CASE("save with string literal fields", "IniFile")
{
    ini::IniFile inif;
    inif["Foo"]["bar1"] = "hello";
    inif["Foo"]["bar2"] = "world";

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1=hello\nbar2=world\n");
}

TEST_CASE("save with custom field sep", "IniFile")
{
    ini::IniFile inif(':', '#');
    inif["Foo"]["bar1"] = true;
    inif["Foo"]["bar2"] = false;

    std::string result = inif.encode();
    REQUIRE(result == "[Foo]\nbar1:true\nbar2:false\n");
}

TEST_CASE("inline comments in sections are discarded", "IniFile")
{
    std::istringstream ss("[Foo] # This is an inline comment\nbar=Hello world!");
    ini::IniFile inif(ss);

    REQUIRE(inif.find("Foo") != inif.end());
}

TEST_CASE("inline comments in fields are discarded", "IniFile")
{
    std::istringstream ss("[Foo]\n"
                          "bar=Hello #world!");
    ini::IniFile inif(ss);

    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "Hello");
}

TEST_CASE("inline comments can be escaped", "IniFile")
{
    std::istringstream ss("[Foo]\n"
                          "bar=Hello \\#world!");
    ini::IniFile inif(ss);

    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "Hello #world!");
}

TEST_CASE("escape characters are kept if not before a comment prefix", "IniFile")
{
    std::istringstream ss("[Foo]\n"
                          "bar=Hello \\world!");
    ini::IniFile inif(ss);

    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "Hello \\world!");
}

/***************************************************
 *                Failing Tests
 ***************************************************/

TEST_CASE("fail to load unclosed section", "IniFile")
{
    ini::IniFile inif;
    REQUIRE_THROWS(inif.decode("[Foo\nbar=bla"));
}

TEST_CASE("fail to load field without equal", "IniFile")
{
    ini::IniFile inif;
    REQUIRE_THROWS(inif.decode("[Foo]\nbar"));
}

TEST_CASE("fail to parse as bool", "IniFile")
{
    std::istringstream ss("[Foo]\nbar=bla");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 1);
    REQUIRE_THROWS(inif["Foo"]["bar"].as<bool>());
}

TEST_CASE("fail to parse as int", "IniFile")
{
    std::istringstream ss("[Foo]\nbar=bla");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 1);
    REQUIRE_THROWS(inif["Foo"]["bar"].as<int>());
}

TEST_CASE("fail to parse as double", "IniFile")
{
    std::istringstream ss("[Foo]\nbar=bla");
    ini::IniFile inif(ss);

    REQUIRE(inif.size() == 1);
    REQUIRE(inif["Foo"].size() == 1);
    REQUIRE_THROWS(inif["Foo"]["bar"].as<double>());
}

TEST_CASE("fail to parse field without section", "IniFile")
{
    ini::IniFile inif;
    REQUIRE_THROWS(inif.decode("bar=bla"));
}

TEST_CASE("spaces are not taken into account in field names", "IniFile")
{
    std::istringstream ss(("[Foo]\n  \t  bar  \t  =hello world"));
    ini::IniFile inif(ss);

    REQUIRE(inif["Foo"].find("bar") != inif["Foo"].end());
    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "hello world");
}

TEST_CASE("spaces are not taken into account in field values", "IniFile")
{
    std::istringstream ss(("[Foo]\nbar=  \t  hello world  \t  "));
    ini::IniFile inif(ss);

    REQUIRE(inif["Foo"]["bar"].as<std::string>() == "hello world");
}

TEST_CASE("spaces are not taken into account in sections", "IniFile")
{
    std::istringstream ss("  \t  [Foo]  \t  \nbar=bla");
    ini::IniFile inif(ss);

    REQUIRE(inif.find("Foo") != inif.end());
}
