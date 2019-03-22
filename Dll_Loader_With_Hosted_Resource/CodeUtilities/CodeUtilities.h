#pragma once
///////////////////////////////////////////////////////////////////////
// CodeUtilities.h - small, generally useful, helper classes         //
// ver 1.2                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes:
* - ProcessCmdLine    extracts path, options, patterns, and a number from command line
* - Converter<T>      converts T to and from strings
* - Box<T>            converts primitive type to instance of a class
* - PersistFactory<T> adds toXml() method to T
*
* Build Process:
* --------------
* Required Files: 
*   CodeUtilities.h
*
* Maintenance History:
* --------------------
* ver 1.3 : 14 Sep 2018
* - added comments for ProcessCmdLine methods
* ver 1.2 : 11 Aug 2018
* - added ProcessCmdLine::hasOption method
* - fixed bugs in ProcessCmdLine::showCmdLine
*   and ProcessCmdLine::showOptions
* ver 1.1 : 10 Aug 2018
* - added ProcessCmdLine class
* ver 1.0 : 12 Jan 2018
* - first release
* - refactored from earlier Utilities.h
*
* Notes:
* ------
* - Designed to provide all functionality in header file. 
* - Implementation file only needed for test and demo.
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

namespace Utilities
{
  /////////////////////////////////////////////////////////////////////
  // preface function
  // - used to add a string preface to an output, e.g., "\n  "

  inline void preface(const std::string& msg = "", bool doReturn = true, std::ostream& out = std::cout, const std::string& prefix = "  ")
  {
    if (doReturn) out << "\n" << prefix << msg;
  }

  /////////////////////////////////////////////////////////////////////
  // ProcessCmdLine class
  // - extracts path, patterns, options, number
  // - expects path to be first command line argument
  // - patterns are strings of the form *.h or *.cpp
  // - options are single characters preceded by /, e.g., /s

  class ProcessCmdLine
  {
  public:
    using Path = std::string;
    using Option = int;
    using Options = std::vector<Option>;
    using Pattern = std::string;
    using Patterns = std::vector<Pattern>;
    using Number = long int;

    ProcessCmdLine(int argc, char** argv, std::ostream& out = std::cout);
    bool parseError();
    Path path();
    void path(const Path& path);
    Options options();
    void option(Option op);
    bool hasOption(Option op);
    Patterns patterns();
    void pattern(const Pattern& patt);
    Number maxItems();
    void maxItems(Number number);
    void usage();
    void showCmdLine(int argc, char** argv);
    void showCmdLine();
    void showPath();
    void showOptions();
    void showPatterns();
    void showMaxItems();
  private:
    Path path_;
    Patterns patterns_;
    Options options_;
    int maxItems_ = 0;
    bool parseError_ = false;
    std::ostream& out_;
  };

  /*----< path operations >------------------------------------------*/
  /*
  *  - Supports changing, getting, and displaying paths.
  *  - ProcessCmdLine expects a path as the first argument.
  */
  inline void ProcessCmdLine::path(const Path& path) 
  { 
    path_ = path; 
  }

  inline ProcessCmdLine::Path ProcessCmdLine::path()
  {
    return path_;
  }

  inline void ProcessCmdLine::showPath()
  {
    out_ << path_ << " ";
  }

  /*----< options operations >---------------------------------------*/
  /*
  *  - Supports adding, viewing, testing for inclusion, and 
  *    displaying options of the form /s, /r, ...
  *  - The meaning of options are application specific.
  *    I usually reserve /s to specify recursive behavior.
  */
  inline void ProcessCmdLine::option(Option option)
  {
    options_.push_back(option);
  }

  inline ProcessCmdLine::Options ProcessCmdLine::options()
  { 
    return options_; 
  }

  inline bool ProcessCmdLine::hasOption(Option op)
  {
    for (auto item : options_)
    {
      if (item == op)
      {
        return true;
      }
    }
    return false;
  }

  inline void ProcessCmdLine::showOptions()
  {
    for (auto opt : options_)
    {
      out_ << '/' << char(opt) << " ";
    }
  }

  /*----< patterns operations >--------------------------------------*/
  /*
  *  - Supports adding, getting, and viewing patterns.
  *  - Patterns are strings used to qualify searches, e.g., *.h, *.cpp.
  *  - If application understands regular expressions, patterns could
  *    be used to pass them.
  *  - Any command line argument that is not the first (a path) or
  *    an option (starts with /), or an integer, is assumed to be a
  *    pattern.  So patterns can appear anywhere on the command line
  *    except in the first position
  */
  inline void ProcessCmdLine::pattern(const Pattern& pattern)
  {
    patterns_.push_back(pattern);
  }

  inline ProcessCmdLine::Patterns ProcessCmdLine::patterns()
  {
    return patterns_;
  }

  inline void ProcessCmdLine::showPatterns()
  {
    for (auto patt : patterns_)
    {
      out_ << patt << " ";
    }
  }

  /*----< maxItems operations >--------------------------------------*/
  /*
  *  - Supports changing, getting, and viewing a maxItems value.
  *  - A maxItem is a non-negative integer used to specify the
  *    maximum number of items to process.
  *  - Items are application specific.
  */
  inline void ProcessCmdLine::maxItems(Number maxItems)
  {
    maxItems_ = maxItems;
  }

  inline ProcessCmdLine::Number ProcessCmdLine::maxItems()
  {
    return maxItems_;
  }

  inline void ProcessCmdLine::showMaxItems()
  {
    if(maxItems_ != 0)
      out_ << maxItems_ << " ";
  }

  /*----< parseError operation >-------------------------------------*/
  /*
  *  - Parse errors occur if the command line has no arguments.
  *  - Parse error is set by the ProcessCmdLine constructor.
  */
  inline bool ProcessCmdLine::parseError()
  {
    return parseError_;
  }

  /*----< command line operations >----------------------------------*/
  /*
  *  - The ProcessCmdLine constructor is passed the arguments of main
  *    and an option specification of an output stream, ostream by default.
  *  - ShowCmdLine(argc, argv) displays the command line as passed to 
  *    the constructor.
  *  - ShowCmdLine() displays the command line with any additions made
  *    using the methods above.
  *  - Usage() displays a message about the expected command line 
  *    arguments.  It is usually called by an application when parseError()
  *    returns true.
  */
  inline ProcessCmdLine::ProcessCmdLine(int argc, char** argv, std::ostream& out) : out_(out)
  {
    if (argc < 2)
    {
      out << "\n  command line parse error\n";
      usage();
      parseError_ = true;
      return;
    }

    path_ = argv[1];

    for (int i = 2; i < argc; ++i)
    {
      if (argv[i][0] == '/')
      {
        if (strlen(argv[i]) > 2)
          continue;
        options_.push_back(argv[i][1]);
      }
      else
      {
        int number = atoi(argv[i]);
        if (number > 0)
        {
          maxItems_ = number;
        }
        else
        {
          patterns_.push_back(argv[i]);
        }
      }
    }
  }

  inline void ProcessCmdLine::showCmdLine(int argc, char** argv)
  {
    for (int i = 0; i < argc; ++i)
    {
      if (argv[i] == "0")
        continue;
      if(argv[i][0] == '/')
      { 
        out_ << "/" + char(argv[i][1]) << " ";
      }
      else
      {
        out_ << argv[i] << " ";
      }
    }
  }

  inline void ProcessCmdLine::showCmdLine()
  {
    showPath();
    preface("", false);
    showOptions();
    preface("", false);
    showPatterns();
    preface("", false);
    showMaxItems();
  }

  inline void ProcessCmdLine::usage()
  {
    out_ << "\n  Command Line: path [/option]* [/pattern]* [integer]";
    out_ << "\n    path is relative or absolute path where processing begins";
    out_ << "\n    [/option]* are one or more options of the form /s, /r, etc.";
    out_ << "\n    [pattern]* are one or more pattern strings used for matching";
    out_ << "\n    [integer] is the maximum number of items to process";
    out_ << "\n";
  }

  /////////////////////////////////////////////////////////////////////
  // Converter class
  // - supports converting unspecified types to and from strings
  // - a type is convertible if it provides insertion and extraction
  //   operators

  template <typename T>
  class Converter
  {
  public:
    static std::string toString(const T& t);
    static T toValue(const std::string& src);
  };
  //----< convert t to a string >--------------------------------------

  template <typename T>
  std::string Converter<T>::toString(const T& t)
  {
    std::ostringstream out;
    out << t;
    return out.str();
  }
  //----< convert a string to an instance of T >-----------------------
  /*
  *  - the string must have been generated by Converter<T>::toString(const T& t)
  *  - T::operator>> must be the inverse of T::operator<< 
  */
  template<typename T>
  T Converter<T>::toValue(const std::string& src)
  {
    std::istringstream in(src);
    T t;
    in >> t;
    return t;
  }

  ///////////////////////////////////////////////////////////////////////
  // Box class
  // - wraps primitive type in class
  // - preserves primitive syntax

  template<typename T>
  class Box
  {
  public:
    Box() : primitive_(T()) {}
    Box(const T& t) : primitive_(t) {}
    operator T&() { return primitive_; }
    T& operator=(const T& t) { primitive_ = t; return primitive_; }
  private:
    T primitive_;
  };

  ///////////////////////////////////////////////////////////////////////
  // ToXml interface
  // - defines language for creating XML elements

  struct ToXml
  {
    virtual std::string toXml(const std::string& tag) = 0;
    virtual ~ToXml() {};
  };

  ///////////////////////////////////////////////////////////////////////
  // PersistFactory<T> class
  // - wraps an instance of user-defined type
  // - preserves semantics of user-defined type
  // - adds toXml("tag") method

  template<typename T>
  class PersistFactory : public T, ToXml
  {
  public:
    PersistFactory() = default;
    PersistFactory(const T& t)
    {
      T::operator=(t);
    }
    std::string toXml(const std::string& tag)
    {
      std::ostringstream out;
      out << "<" << tag << ">" << *this << "</" << tag << ">";
      return out.str();
    }
  };
}