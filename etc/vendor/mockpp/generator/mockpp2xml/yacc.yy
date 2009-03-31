
%{
/*****************************************************************
Copyright (c) 1999 Torben Weis <weis@kde.org>
Copyright (c) 2000 Matthias Ettrich <ettrich@kde.org>
Copyright (c) 2007 Ewald Arnold <mockpp@ewald-arnold.de>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/

//#include <config.h>

// Workaround for a bison issue:
// bison.simple concludes from _GNU_SOURCE that stpcpy is available,
// while GNU string.h only exposes it if __USE_GNU is set.
#ifdef _GNU_SOURCE
#define __USE_GNU 1
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "mstring.h"

#define AMP_ENTITY "&amp;"
#define YYERROR_VERBOSE

extern int yylex();

// extern MString idl_lexFile;
extern int idl_line_no;
extern int function_mode;

static int mocking_area = 0;
static int mockpp_methods_area = 0;
static int mockpp_constructor_area = 0;

static MString in_namespace( "" );

void dcopidlInitFlex( const char *_code );

void yyerror( const char *s )
{
  fprintf(stderr, "In line %i : %s\n", idl_line_no, s );
        exit(1);
  //   theParser->parse_error( idl_lexFile, s, idl_line_no );
}

%}


%union
{
  long                 _int;
  MString             *_str;
  unsigned short       _char;
  double               _float;
}

%nonassoc T_UNIMPORTANT
%token <_char> T_CHARACTER_LITERAL
%token <_float> T_DOUBLE_LITERAL
%right <_str> T_IDENTIFIER
%token <_int> T_INTEGER_LITERAL
%token <_str> T_STRING_LITERAL
%token <_str> T_INCLUDE
%token T_CLASS
%token T_STRUCT
%token T_LEFT_CURLY_BRACKET
%token T_LEFT_PARANTHESIS
%token T_RIGHT_CURLY_BRACKET
%token T_RIGHT_PARANTHESIS
%token T_COLON
%token T_SEMICOLON
%token T_PUBLIC
%token T_PROTECTED
%token T_TRIPE_DOT
%token T_PRIVATE
%token T_VIRTUAL
%token T_CONST
%token T_THROW
%token T_INLINE
%token T_FRIEND
%token T_RETURN
%token T_SIGNAL
%token T_SLOT
%token T_TYPEDEF
%token T_PLUS
%token T_MINUS
%token T_COMMA
%right T_ASTERISK
%token T_TILDE
%token T_LESS
%token T_GREATER
%token T_AMPERSAND
%token T_EXTERN
%token T_EXTERN_C
%token T_ACCESS
%token T_ENUM
%token T_NAMESPACE
%token T_USING
%token T_UNKNOWN
%token T_TRIPLE_DOT
%token T_TRUE
%token T_FALSE
%token T_STATIC
%token T_MUTABLE
%token T_EQUAL
%token T_SCOPE
%token T_NULL
%token T_INT
%token T_ARRAY_OPEN
%token T_ARRAY_CLOSE
%token T_CHAR
%token T_ULXRMETHODS_AREA
%token T_ULXRFUNCTION
%token T_ULXRCTOR_AREA
%token T_MOCKMETHODS_AREA
%token T_MOCKCTOR_AREA
%token T_SIGNED
%token T_UNSIGNED
%token T_LONG
%token T_SHORT
%token T_FUNOPERATOR
%token T_MISCOPERATOR
%token T_SHIFT
%token T_DCOP
%token T_DCOP_AREA
%token T_DCOP_SIGNAL_AREA
%token T_QOBJECT

%type <_str> body
%type <_str> class_header
%type <_str> super_classes
%type <_str> super_class
%type <_str> super_class_name
%type <_str> typedef
%type <_str> function
%type <_str> function_header
%type <_str> param
%type <_str> type
%type <_str> type_name
%type <_str> templ_type
%type <_str> templ_type_list
%type <_str> type_list
%type <_str> params
%type <_str> vparams
%type <_str> int_type
%type <_int> const_qualifier
%type <_str> throw_qualifier
%type <_int> virtual_qualifier
%type <_str> Identifier
%type <_int> platformtag
%type <_str> asterisks

%%

/*1*/
main
  : includes declaration main
    {
    }
  | /* empty */
  ;

includes
  : includes T_INCLUDE
    {
      printf("<INCLUDE>%s</INCLUDE>\n", $2->latin1() );
    }
  | T_EXTERN_C T_LEFT_CURLY_BRACKET main T_RIGHT_CURLY_BRACKET
    {
    }
  | /* empty */
    {
    }
  ;

dcop_area_begin
  : T_DCOP_AREA T_COLON
  {
    /*empty*/
  }
  ;

dcop_signal_area_begin
  : T_DCOP_SIGNAL_AREA T_COLON
  {
    /*empty*/
  }
  ;

platformtag
  : T_DCOP { $$ = 1; }
  | T_QOBJECT { $$ = 2; }
  | /*empty*/  { $$ = 0; }
  ;

declaration
  : T_CLASS Identifier class_header platformtag body T_SEMICOLON
    {
      printf("<CLASS>\n    <NAME>%s</NAME>\n%s%s</CLASS>\n", MString( in_namespace + *$2 ).latin1(), $3->latin1(), $5->latin1() );
      mocking_area = 0;
    }
  | T_CLASS T_IDENTIFIER Identifier class_header platformtag body T_SEMICOLON
    {
      printf("<CLASS>\n    <NAME>%s</NAME>\n    <LINK_SCOPE>%s</LINK_SCOPE>\n%s%s</CLASS>\n", MString( in_namespace + *$3 ).latin1(), $2->latin1(),  $4->latin1(), $6->latin1() );
      mocking_area = 0;

    }
  | T_CLASS Identifier T_SEMICOLON
    {
    }
  | T_STRUCT Identifier T_SEMICOLON
    {
    }
  | T_STRUCT Identifier class_header body T_SEMICOLON
    {
    }
  | T_NAMESPACE T_IDENTIFIER T_LEFT_CURLY_BRACKET
      {
          in_namespace += *$2; in_namespace += "::";
      }
      main T_RIGHT_CURLY_BRACKET opt_semicolon
        {
            std::size_t pos = in_namespace.findRev( "::", -3 );
            if( pos != MString::npos )
                in_namespace = in_namespace.left( pos + 2 );
            else
                in_namespace = "";
        }
  | T_USING T_NAMESPACE T_IDENTIFIER T_SEMICOLON
    {
    }
  | T_USING T_IDENTIFIER T_SCOPE T_IDENTIFIER T_SEMICOLON
    {
    }
  | T_EXTERN T_SEMICOLON
    {
    }
  | T_TYPEDEF type Identifier T_SEMICOLON
    {
    }
  | T_TYPEDEF T_STRUCT T_LEFT_CURLY_BRACKET member_list T_RIGHT_CURLY_BRACKET Identifier T_SEMICOLON
    {
    }
  | T_TYPEDEF T_STRUCT Identifier T_LEFT_CURLY_BRACKET member_list T_RIGHT_CURLY_BRACKET Identifier T_SEMICOLON
    {
    }
  | T_INLINE function
    {
    }
  | function
    {
    }
  | member
    {
    }
  | enum
    {
    }
  ;

member_list
  : member member_list
  | /* empty */
  ;

bool_value: T_TRUE | T_FALSE;

access_kw: T_PRIVATE | T_PROTECTED | T_PUBLIC
  {
    mocking_area = 0;
  }
  ;

sigslot: T_SIGNAL | T_SLOT;

signal_area_begin
  : access_kw sigslot T_COLON
  {
  }
  ;
  | sigslot T_COLON
  {
  }
  ;

access_kw_begin
  : access_kw T_COLON
  {
    mocking_area = 0;
  }
  ;

mockpp_methods_area_begin
  : T_MOCKMETHODS_AREA T_COLON
  {
    mockpp_methods_area = 1;
    mocking_area = 1;
  }
  | access_kw T_MOCKMETHODS_AREA T_COLON
  {
    mockpp_methods_area = 1;
    mocking_area = 1;
  }
  ;

mockpp_constructor_area_begin
  : T_MOCKCTOR_AREA T_COLON
  {
    mockpp_constructor_area = 1;
    mocking_area = 1;
  }
  | access_kw T_MOCKCTOR_AREA T_COLON
  {
    mockpp_constructor_area = 1;
    mocking_area = 1;
  }
  ;

Identifier
  : T_IDENTIFIER %prec T_UNIMPORTANT
    {
      $$ = $1;
    }
  | T_IDENTIFIER T_SCOPE Identifier
    {
      MString* tmp = new MString( "%1::%2" );
            *tmp = tmp->arg(*($1)).arg(*($3));
            $$ = tmp;
    }
  ;

super_class_name
  : Identifier
    {
      MString* tmp = new MString( "    <SUPER>%1</SUPER>\n" );
      *tmp = tmp->arg( *($1) );
      $$ = tmp;
    }
  | Identifier T_LESS type_list T_GREATER
    {
      MString* tmp = new MString( "    <SUPER>%1</SUPER>\n" );
      *tmp = tmp->arg( *($1) + "&lt;" + *($3) + "&gt;" );
      $$ = tmp;
    }
  ;

super_class
  : virtual_qualifier T_PUBLIC super_class_name
    {
      $$ = $3;
    }
  | super_class_name
    {
      $$ = $1;
    }
  ;

super_classes
  : super_class T_LEFT_CURLY_BRACKET
    {
      $$ = $1;
    }
  | super_class T_COMMA super_classes
    {
      $$ = new MString( *($1) + *($3) );
    }
  ;

class_header
  : T_COLON super_classes
    {
      $$ = $2;
    }
  | T_LEFT_CURLY_BRACKET
    {
      $$ = new MString( "" );
    }
  ;

opt_semicolon
  : /* empty */
    {
    }
  | T_SEMICOLON
  ;

body
  : T_RIGHT_CURLY_BRACKET
    {
      $$ = new MString( "" );
    }
  | typedef body
    {
      $$ = new MString( *($1) + *($2) );
    }
  | T_INLINE function body
    {
      $$ = new MString( *($2) + *($3) );
    }
  | function body
    {
      $$ = new MString( *($1) + *($2) );
    }
  | mockpp_methods_area_begin body
    {
      $$ = $2;
    }
  | mockpp_constructor_area_begin body
    {
      $$ = $2;
    }
  | enum body
    {
      $$ = $2;
    }
  | access_kw_begin body
    {
      $$ = $2;
    }
  | dcop_area_begin body
    {
      $$ = $2;
    }
  | signal_area_begin body
    {
      $$ = $2;
    }
  | dcop_signal_area_begin body
    {
      $$ = $2;
    }
  | member body
    {
      $$ = $2;
    }
  | T_FRIEND T_CLASS Identifier T_SEMICOLON body
    {
      $$ = $5;
    }
  | T_FRIEND Identifier T_SEMICOLON body
    {
      $$ = $4;
    }
  | T_FRIEND function_header T_SEMICOLON body
    {
      $$ = $4;
    }
  | T_CLASS Identifier T_SEMICOLON body
    {
      $$ = $4;
    }
  | T_CLASS Identifier class_header body T_SEMICOLON body
    {
      $$ = $6;
    }
  | T_STRUCT Identifier T_SEMICOLON body
    {
      $$ = $4;
    }
  | T_STRUCT Identifier class_header body T_SEMICOLON body
    {
      $$ = $6;
    }
  | T_USING T_IDENTIFIER T_SCOPE T_IDENTIFIER T_SEMICOLON body
    {
      $$ = $6;
    }
  ;

enum
  : T_ENUM T_IDENTIFIER T_LEFT_CURLY_BRACKET enum_list T_RIGHT_CURLY_BRACKET T_IDENTIFIER T_SEMICOLON
  | T_ENUM T_IDENTIFIER T_LEFT_CURLY_BRACKET enum_list T_RIGHT_CURLY_BRACKET T_SEMICOLON
  | T_ENUM T_LEFT_CURLY_BRACKET enum_list T_RIGHT_CURLY_BRACKET T_IDENTIFIER T_SEMICOLON
  | T_ENUM T_LEFT_CURLY_BRACKET enum_list T_RIGHT_CURLY_BRACKET T_SEMICOLON
  ;

enum_list
  : enum_item T_COMMA enum_list
  | enum_item
  ;

enum_item
  : T_IDENTIFIER T_EQUAL int_expression {}
  | T_IDENTIFIER {}
  ;

number
  : T_CHARACTER_LITERAL {}
  | T_INTEGER_LITERAL {}
  | T_MINUS T_INTEGER_LITERAL {}
  | T_NULL {}
  | Identifier {}
  ;

int_expression
  : number {}
  | number T_PLUS number {}
  | number T_SHIFT number {}
  ;

typedef
  : T_TYPEDEF Identifier T_LESS type_list T_GREATER Identifier T_SEMICOLON
    {
      if (mocking_area)
      {
        MString* tmp = new MString("<TYPEDEF name=\"%1\" template=\"%2\">%3</TYPEDEF>\n");
        *tmp = tmp->arg( *($6) ).arg( *($2) ).arg( *($4) );
        $$ = tmp;
      } else
      {
        $$ = new MString("");
      }
    }
  | T_TYPEDEF Identifier T_LESS type_list T_GREATER T_SCOPE T_IDENTIFIER Identifier T_SEMICOLON
    {
      if (mocking_area)
        yyerror("scoped template typedefs are not supported in mock methods!");
    }
  ;

throw_qualifier
  : /* empty */
    {
        $$ = new MString("");
    }
  | T_THROW T_LEFT_PARANTHESIS params T_RIGHT_PARANTHESIS
    {
        $$ = new MString("/* todo */");
    }
  ;

const_qualifier
  : /* empty */
    {
      $$ = 0;
    }
  | T_CONST
    {
      $$ = 1;
    }
  ;

int_type
  : T_SIGNED { $$ = new MString("signed int"); }
  | T_SIGNED T_INT { $$ = new MString("signed int"); }
  | T_UNSIGNED { $$ = new MString("unsigned int"); }
  | T_UNSIGNED T_INT { $$ = new MString("unsigned int"); }
  | T_SIGNED T_SHORT { $$ = new MString("signed short int"); }
  | T_SIGNED T_SHORT T_INT { $$ = new MString("signed short int"); }
  | T_SIGNED T_LONG { $$ = new MString("signed long int"); }
  | T_SIGNED T_LONG T_INT { $$ = new MString("signed long int"); }
  | T_UNSIGNED T_SHORT { $$ = new MString("unsigned short int"); }
  | T_UNSIGNED T_SHORT T_INT { $$ = new MString("unsigned short int"); }
  | T_UNSIGNED T_LONG { $$ = new MString("unsigned long int"); }
  | T_UNSIGNED T_LONG T_INT { $$ = new MString("unsigned long int"); }
  | T_INT { $$ = new MString("int"); }
  | T_LONG { $$ = new MString("long int"); }
  | T_LONG T_INT { $$ = new MString("long int"); }
  | T_SHORT { $$ = new MString("short int"); }
  | T_SHORT T_INT { $$ = new MString("short int"); }
  | T_CHAR { $$ = new MString("char"); }
  | T_SIGNED T_CHAR { $$ = new MString("signed char"); }
  | T_UNSIGNED T_CHAR { $$ = new MString("unsigned char"); }
  ;

asterisks
  : T_ASTERISK asterisks
    {
      $$ = new MString( MString("*") + *($2) );
    }
  | T_ASTERISK
    {
      $$ = new MString("*");
    }
  ;

vparams
  : value
    {
    }
  | vparams T_COMMA value
    {
      $$ = new MString( *($1) );
    }
  ;

params
  : /* empty */
    {
      $$ = new MString( "" );
    }
  | param
  | params T_COMMA param
    {
      $$ = new MString( *($1) + *($3) );
    }
  ;

/* Lowlevel definition of a type - doesn't include const, * nor & */
type_name
  : int_type
  | Identifier { $$ = $1; }
  | T_STRUCT Identifier { $$ = $2; }
  | T_CLASS Identifier { $$ = $2; }
  | Identifier T_LESS templ_type_list T_GREATER
    {
      MString *tmp = new MString("%1&lt;%2&gt;");
      *tmp = tmp->arg(*($1));
      *tmp = tmp->arg(*($3));
      $$ = tmp;
    }
  | Identifier T_LESS templ_type_list T_GREATER T_SCOPE Identifier
    {
      MString *tmp = new MString("%1&lt;%2&gt;::%3");
      *tmp = tmp->arg(*($1));
      *tmp = tmp->arg(*($3));
      *tmp = tmp->arg(*($6));
      $$ = tmp;
    }
  ;

/* List of types inside a template def like QMap< blah, blah > */
templ_type_list
  : templ_type T_COMMA templ_type_list
    {
      $$ = new MString(*($1) + "," + *($3));
    }
  | templ_type
    {
      $$ = $1;
    }
  ;

/* One type inside a template. No '&' or const here. */
templ_type
  : type_name asterisks
    {
      if (mocking_area)
        yyerror("in mock methods are no pointers allowed");
    }
  | type_name
    {
      $$ = $1;
    }
  ;

/* The hi-level, complete definition of a type, which also generates
   the <TYPE> tag for it */
type
    : T_CONST type_name asterisks
      {
         if (mocking_area)
         {
            MString* tmp = new MString("<TYPE  qleft=\"const\" qright=\"%2\">%1</TYPE>");
            *tmp = tmp->arg( *($2) );
            *tmp = tmp->arg( *($3) );
            $$ = tmp;
         }
      }
    | T_CONST type_name T_AMPERSAND
      {
         if (mocking_area)
         {
            MString* tmp = new MString("<TYPE  qleft=\"const\" qright=\"" AMP_ENTITY "\">%1</TYPE>");
            *tmp = tmp->arg( *($2) );
            $$ = tmp;
         }
      }
    | T_CONST type_name %prec T_UNIMPORTANT
      {
        MString* tmp = new MString("<TYPE qleft=\"const\" >%1</TYPE>");
        *tmp = tmp->arg( *($2) );
        $$ = tmp;
      }
    | type_name T_CONST %prec T_UNIMPORTANT
      {
        MString* tmp = new MString("<TYPE qleft=\"const\">%1</TYPE>");
        *tmp = tmp->arg( *($1) );
        $$ = tmp;
      }
    | type_name T_CONST T_AMPERSAND
      {
        if (mocking_area)
        {
           MString* tmp = new MString("<TYPE qleft=\"const\" qright=\"" AMP_ENTITY "\">%1</TYPE>");
           *tmp = tmp->arg( *($1) );
           $$ = tmp;
        }
      }
    | type_name T_AMPERSAND
      {
        if (mocking_area)
        {
           MString* tmp = new MString("<TYPE qright=\"" AMP_ENTITY "\">%1</TYPE>");
           *tmp = tmp->arg( *($1) );
           $$ = tmp;
        }
      }
    | type_name %prec T_UNIMPORTANT
      {
        MString* tmp = new MString("<TYPE>%1</TYPE>");
        *tmp = tmp->arg( *($1) );
        $$ = tmp;
      }
    | type_name asterisks T_CONST
      {
        if (mocking_area)
        {
           MString* tmp = new MString("<TYPE qright=\"%2 const\">%1</TYPE>");
           *tmp = tmp->arg( *($1) );
           *tmp = tmp->arg( *($2) );
           $$ = tmp;
        }
      }
    | type_name asterisks
      {
        if (mocking_area)
        {
           MString* tmp = new MString("<TYPE qright=\"%2\">%1</TYPE>");
           *tmp = tmp->arg( *($1) );
           *tmp = tmp->arg( *($2) );
           $$ = tmp;
        }
      }
    ;

type_list
  : type T_COMMA type_list
    {
      $$ = new MString(*($1) + "," + *($3));
    }
  | type
    {
       $$ = $1;
    }
  ;

param
  : type Identifier default
    {
      if (mocking_area)
      {
        MString* tmp = new MString("\n        <ARG>%1<NAME>%2</NAME></ARG>");
        *tmp = tmp->arg( *($1) );
        *tmp = tmp->arg( *($2) );
        $$ = tmp;
      }
      else
        $$ = new MString();
    }
  | type default
    {
      if (mocking_area)
      {
         MString* tmp = new MString("\n        <ARG>%1</ARG>");
         *tmp = tmp->arg( *($1) );
         $$ = tmp;
      }
      else
        $$ = new MString();
    }
  | T_TRIPLE_DOT
    {
      if (mocking_area)
        yyerror("variable arguments not supported in mock methods.");
      $$ = new MString("");
    }
  ;

default
  : /* empty */
    {
    }
  | T_EQUAL value
    {
    }
  | T_EQUAL T_LEFT_PARANTHESIS type T_RIGHT_PARANTHESIS value /* cast */
    {
    }
  ;

/* Literal or calculated value, used for an initialization */
value
  : T_STRING_LITERAL
    {
    }
  | int_expression
    {
    }
  | T_DOUBLE_LITERAL
    {
    }
  | bool_value
    {
    }
  | Identifier T_LEFT_PARANTHESIS vparams T_RIGHT_PARANTHESIS  // ea: params
    {
    }
  ;

virtual_qualifier
  : /* empty */ { $$ = 0;  }
  | T_VIRTUAL { $$ = 1; }
  ;

operator
  : T_MISCOPERATOR | T_SHIFT | T_GREATER | T_LESS | T_EQUAL
  ;

function_header
  : type Identifier T_LEFT_PARANTHESIS params T_RIGHT_PARANTHESIS const_qualifier throw_qualifier
    {
      if (mocking_area)
      {
        MString* tmp = 0;
        tmp = new MString(
                "    <%4>\n"
                "        %2\n"
                "        <NAME>%1</NAME>"
                "%3\n"
                "    </%5>\n");
        *tmp = tmp->arg( *($2) );
        *tmp = tmp->arg( *($1) );
        *tmp = tmp->arg( *($4) );

        MString tagname = "METHOD";

        MString attr = ($6) ? " qual=\"const\"" : "";
        MString thrower (*($7));
        if (thrower.length() != 0)
          attr += MString(" throw='%1'").arg(thrower);
        *tmp = tmp->arg( MString("%1%2").arg(tagname).arg(attr) );
        *tmp = tmp->arg( MString("%1").arg(tagname) );
        $$ = tmp;
      }
      else
        $$ = new MString("");
    }
  | T_VIRTUAL type Identifier T_LEFT_PARANTHESIS params T_RIGHT_PARANTHESIS const_qualifier throw_qualifier
    {
      if (mocking_area)
      {
        MString* tmp = 0;
        tmp = new MString(
                "    <%4>\n"
                "        %2\n"
                "        <NAME>%1</NAME>"
                "%3\n"
                "    </%5>\n");
        *tmp = tmp->arg( *($3) );
        *tmp = tmp->arg( *($2) );
        *tmp = tmp->arg( *($5) );

        MString tagname = "METHOD";
        MString attr = ($7) ? " qual=\"const\"" : "";
        MString thrower (*($8));
        if (thrower.length() != 0)
          attr += MString(" throw='%1'").arg(thrower);
        attr += " virtual='1'";
        *tmp = tmp->arg( MString("%1%2").arg(tagname).arg(attr) );
        *tmp = tmp->arg( MString("%1").arg(tagname) );
        $$ = tmp;
      }
      else
        $$ = new MString("");
    }
  | T_VIRTUAL type Identifier T_LEFT_PARANTHESIS params T_RIGHT_PARANTHESIS const_qualifier throw_qualifier T_EQUAL T_NULL
    {
      if (mocking_area)
      {
        MString* tmp = 0;
        tmp = new MString(
                "    <%4>\n"
                "        %2\n"
                "        <NAME>%1</NAME>"
                "%3\n"
                "    </%5>\n");
        *tmp = tmp->arg( *($3) );
        *tmp = tmp->arg( *($2) );
        *tmp = tmp->arg( *($5) );

        MString tagname = "METHOD";

        MString attr = ($7) ? " qual=\"const\"" : "";
        attr += " virtual='1' pure='1'";
        MString thrower (*($8));
        if (thrower.length() != 0)
          attr += MString(" throw='%1'").arg(thrower);
        *tmp = tmp->arg( MString("%1%2").arg(tagname).arg(attr) );
        *tmp = tmp->arg( MString("%1").arg(tagname) );
        $$ = tmp;
      }
      else
       $$ = new MString("");
    }
  | type T_FUNOPERATOR operator T_LEFT_PARANTHESIS params T_RIGHT_PARANTHESIS const_qualifier throw_qualifier
    {
      if (mocking_area)
        yyerror("operators aren't allowed in mock methods!");
       $$ = new MString("");
    }
  ;

/* In an inline constructor:
   List of values used as initialization for member var, or as params to parent constructor */
values
  : value {}
  | value T_COMMA values {}
  | /* empty */ {}
  ;

/* One initialization done by an inline constructor */
init_item
  : T_IDENTIFIER T_LEFT_PARANTHESIS values T_RIGHT_PARANTHESIS {}
  ;

/* List of initializations done by an inline constructor */
init_list
  : init_item {}
  | init_item T_COMMA init_list {}
  ;

function
  : function_header function_body
    {
          $$ = $1;
    }
/*
    | T_VIRTUAL function_header T_EQUAL T_NULL function_body
    {
    $$ = $1;
    }
  | T_VIRTUAL function_header function_body
    {
    $$ = $1;
    }
*/
  | Identifier T_LEFT_PARANTHESIS params T_RIGHT_PARANTHESIS function_body
    {
      /* The constructor */
//      assert(!mockpp_methods_area);
      if (mockpp_constructor_area)
      {
        MString* tmp = 0;
        tmp = new MString(
                "    <%4>"
                "%2\n"
                "        <NAME>%1</NAME>\n"
                "    </%3>\n");
        *tmp = tmp->arg( *($1) );
        if (($3)->length() != 0)
          *tmp = tmp->arg( *($3) );
        else
          *tmp = tmp->arg("");

        MString tagname = "CONSTRUCTOR";
        mocking_area = 0;
        mockpp_constructor_area = 0;  // only once

        *tmp = tmp->arg(tagname).arg(tagname);
        $$ = tmp;
      }
      else
        $$ = new MString("");
    }
  | Identifier T_LEFT_PARANTHESIS params T_RIGHT_PARANTHESIS T_COLON init_list function_body
    {
      if (mockpp_constructor_area)
      {
        MString* tmp = 0;
        tmp = new MString(
                "    <%4>"
                "%2\n"
                "        <NAME>%1</NAME>\n"
                "    </%3>\n");
        *tmp = tmp->arg( *($1) );
        if (($3)->length() != 0)
          *tmp = tmp->arg( *($3));
        else
          *tmp = tmp->arg("");


        MString tagname = "CONSTRUCTOR";
        mocking_area = 0;
        mockpp_constructor_area = 0;  // only once

        *tmp = tmp->arg(tagname).arg(tagname);
        $$ = tmp;
      }
      else
        $$ = new MString("");
    }
  | virtual_qualifier T_TILDE Identifier T_LEFT_PARANTHESIS T_RIGHT_PARANTHESIS function_body
    {
        /* The destructor */
//        assert(!mockpp_methods_area);
//        assert(!mockpp_constructor_area);
        $$ = new MString("");
    }
  | T_STATIC function_header function_body
    {
      if (mocking_area)
         yyerror("mock methods cannot be static");

      else
      {
         $$ = new MString();
      }
    }
  ;

function_begin : T_LEFT_CURLY_BRACKET
  {
    function_mode = 1;
  }
  ;

function_body
  : T_SEMICOLON
  | function_begin function_lines T_RIGHT_CURLY_BRACKET
  | function_begin function_lines T_RIGHT_CURLY_BRACKET T_SEMICOLON
  ;

function_lines
  : function_line function_lines {}
  | T_ASTERISK
  | /* empty */ {}
  ;

function_line
  : T_SEMICOLON /* dummy */
  ;

Identifier_list_rest
  : T_COMMA Identifier_list
  | /* empty */
  ;

Identifier_list_entry : T_IDENTIFIER {}
                      | T_IDENTIFIER T_EQUAL value {}
                      | asterisks T_IDENTIFIER {}
  ;

Identifier_list : Identifier_list_entry Identifier_list_rest {}
  ;

member
  : type Identifier_list T_SEMICOLON {}
  | type Identifier T_COLON T_INTEGER_LITERAL T_SEMICOLON {}
  | T_STATIC type T_IDENTIFIER default T_SEMICOLON {}
  | T_MUTABLE type T_IDENTIFIER default T_SEMICOLON {}
  | type T_IDENTIFIER T_ARRAY_OPEN int_expression T_ARRAY_CLOSE T_SEMICOLON {}
  | T_STATIC type T_IDENTIFIER T_ARRAY_OPEN int_expression T_ARRAY_CLOSE T_SEMICOLON {}
  ;

%%

void dcopidlParse( const char *_code )
{
    mocking_area = 0;
    mockpp_methods_area = 0;
    mockpp_constructor_area = 0;
    dcopidlInitFlex( _code );
    yyparse();
}
