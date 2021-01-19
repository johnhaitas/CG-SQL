(window.webpackJsonp=window.webpackJsonp||[]).push([[35],{126:function(e,t,n){"use strict";n.d(t,"a",(function(){return b})),n.d(t,"b",(function(){return h}));var a=n(0),r=n.n(a);function i(e,t,n){return t in e?Object.defineProperty(e,t,{value:n,enumerable:!0,configurable:!0,writable:!0}):e[t]=n,e}function l(e,t){var n=Object.keys(e);if(Object.getOwnPropertySymbols){var a=Object.getOwnPropertySymbols(e);t&&(a=a.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),n.push.apply(n,a)}return n}function o(e){for(var t=1;t<arguments.length;t++){var n=null!=arguments[t]?arguments[t]:{};t%2?l(Object(n),!0).forEach((function(t){i(e,t,n[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(n)):l(Object(n)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(n,t))}))}return e}function c(e,t){if(null==e)return{};var n,a,r=function(e,t){if(null==e)return{};var n,a,r={},i=Object.keys(e);for(a=0;a<i.length;a++)n=i[a],t.indexOf(n)>=0||(r[n]=e[n]);return r}(e,t);if(Object.getOwnPropertySymbols){var i=Object.getOwnPropertySymbols(e);for(a=0;a<i.length;a++)n=i[a],t.indexOf(n)>=0||Object.prototype.propertyIsEnumerable.call(e,n)&&(r[n]=e[n])}return r}var s=r.a.createContext({}),u=function(e){var t=r.a.useContext(s),n=t;return e&&(n="function"==typeof e?e(t):o(o({},t),e)),n},b=function(e){var t=u(e.components);return r.a.createElement(s.Provider,{value:t},e.children)},d={inlineCode:"code",wrapper:function(e){var t=e.children;return r.a.createElement(r.a.Fragment,{},t)}},p=r.a.forwardRef((function(e,t){var n=e.components,a=e.mdxType,i=e.originalType,l=e.parentName,s=c(e,["components","mdxType","originalType","parentName"]),b=u(n),p=a,h=b["".concat(l,".").concat(p)]||b[p]||d[p]||i;return n?r.a.createElement(h,o(o({ref:t},s),{},{components:n})):r.a.createElement(h,o({ref:t},s))}));function h(e,t){var n=arguments,a=t&&t.mdxType;if("string"==typeof e||a){var i=n.length,l=new Array(i);l[0]=p;var o={};for(var c in t)hasOwnProperty.call(t,c)&&(o[c]=t[c]);o.originalType=e,o.mdxType="string"==typeof e?e:a,l[1]=o;for(var s=2;s<i;s++)l[s]=n[s];return r.a.createElement.apply(null,l)}return r.a.createElement.apply(null,n)}p.displayName="MDXCreateElement"},90:function(e,t,n){"use strict";n.r(t),n.d(t,"frontMatter",(function(){return l})),n.d(t,"metadata",(function(){return o})),n.d(t,"rightToc",(function(){return c})),n.d(t,"default",(function(){return u}));var a=n(2),r=n(6),i=(n(0),n(126)),l={id:"ch08",title:"Chapter 8: Functions",sidebar_label:"Chapter 8: Functions"},o={unversionedId:"ch08",id:"ch08",isDocsHomePage:!1,title:"Chapter 8: Functions",description:"\x3c!---",source:"@site/../CQL_Guide/ch08.md",slug:"/ch08",permalink:"/cql-guide/ch08",version:"current",lastUpdatedBy:"Diego Astiazar\xe1n",lastUpdatedAt:1611087797,sidebar_label:"Chapter 8: Functions",sidebar:"someSidebar",previous:{title:"Chapter 7: CQL Result Sets",permalink:"/cql-guide/ch07"},next:{title:"Chapter 9: Statements Summary and Error Checking",permalink:"/cql-guide/ch09"}},c=[{value:"Notes on Builtin Functions",id:"notes-on-builtin-functions",children:[]}],s={rightToc:c};function u(e){var t=e.components,n=Object(r.a)(e,["components"]);return Object(i.b)("wrapper",Object(a.a)({},s,n,{components:t,mdxType:"MDXLayout"}),Object(i.b)("p",null,"CQL stored procs have a very simple contract so it is easy to declare procedures and then implement them in regular C, the C functions just have to conform to the contract.  However, there is no notion of functions at all and this makes it very inconvenient to use some external code and is not doing database things and wants to return values.  Even a random number generator or something would be difficult to use because it could not be called in the context of an expression.  To allow for this CQL adds declared functions"),Object(i.b)("p",null,"In an other example of the two-headed nature of CQL, there are two ways to declare functions.  As we have already\nseen you can make function-like procedures and call them like functions simply by making a procedure with an ",Object(i.b)("inlineCode",{parentName:"p"},"out")," parameter. However, there are also cases where it is reasonable to make function calls to external functions of other kinds.  There are three major types of functions you might wish to call."),Object(i.b)("h4",{id:"ordinary-scalar-functions"},"Ordinary Scalar Functions"),Object(i.b)("p",null,"These functions are written in regular C and provide for the ability to do operations on in-memory objects.  For instance,\nyou could create functions that allow you to read and write from a dictionary.  You can declare these functions like so:"),Object(i.b)("pre",null,Object(i.b)("code",Object(a.a)({parentName:"pre"},{className:"language-sql"}),"declare function dict_get_value(dict object, key_ text not null) text;\n")),Object(i.b)("p",null,"Such a function is not known to SQLite and therefore cannot appear in SQL statements.  CQL will enforce this."),Object(i.b)("p",null,"The above function returns a text reference, and, importantly, this is a borrowed reference.  The dictionary\nis presumably holding on to the reference and as long as it is not mutated the reference is valid.  CQL will\nretain this reference as soon as it is stored and release it automatically when it is out of scope.  So, in\nthis case, the dictionary continues to own the object."),Object(i.b)("p",null,"It is also possible to declare functions that create objects.  Such as this example:"),Object(i.b)("pre",null,Object(i.b)("code",Object(a.a)({parentName:"pre"},{className:"language-sql"}),"declare function dict_create() create object;\n")),Object(i.b)("p",null,"This declaration tells CQL that the function will create a new object for our use.  CQL does not retain the\nprovided object, rather assuming ownership of the presumably one reference count the object already has.\nWhen the object goes out of scope it is release as usual."),Object(i.b)("p",null,"If we also declare this procedure:"),Object(i.b)("pre",null,Object(i.b)("code",Object(a.a)({parentName:"pre"},{className:"language-sql"}),"declare procedure dict_add(\n    dict object not null, \n    key_ text not null, \n    value text not null);\n")),Object(i.b)("p",null,"Then with this family of declarations we could write something like this:"),Object(i.b)("pre",null,Object(i.b)("code",Object(a.a)({parentName:"pre"},{className:"language-sql"}),'create proc create_and_init(out dict object not null)\nbegin\n  set dict := dict_create();\n  call dict_add(dict, "k1", "v1");\n  call dict_add(dict, "k2", "v2");\n  if (dict_get_value(dict, "k1") == dict__get_value(dict, "k2)) then\n    call printf("insanity has ensued\\n");\n  end if;\nend;\n')),Object(i.b)("p",null,"Note: Ordinary scalar functions may not use the database in any way, when they are invoked they will not\nbe provided with the database pointer and so they will be unable to do any database operations.  To do\ndatabase operations use regular procedures.  You can create a function-like-procedure using the ",Object(i.b)("inlineCode",{parentName:"p"},"out")," convention\ndiscussed previously."),Object(i.b)("h4",{id:"sql-scalar-functions"},"SQL Scalar Functions"),Object(i.b)("p",null,"SQLite includes the ability to add new functions to its expressions using ",Object(i.b)("inlineCode",{parentName:"p"},"sqlite3_create_function"),".  In\norder to use this function in CQL, you must also provide its prototype definition to the compiler.  You\ncan do so like in this example:"),Object(i.b)("pre",null,Object(i.b)("code",Object(a.a)({parentName:"pre"},{className:"language-sql"}),"declare select function strencode(t text not null) text not null;\n")),Object(i.b)("p",null,"This introduces the function ",Object(i.b)("inlineCode",{parentName:"p"},"strencode")," to the compiler for use in SQL constructs.  With this done you\ncould write a procedure something like this:"),Object(i.b)("pre",null,Object(i.b)("code",Object(a.a)({parentName:"pre"},{className:"language-sql"}),"create table foo(id integer, t text);\n\ncreate procedure bar(id_ integer)\nbegin\n   select strencode(T1.t) from foo T1 where T1.id = id_;\nend;\n")),Object(i.b)("p",null,'This presumably returns the "encoded" text, whatever that might be.  Note that if ',Object(i.b)("inlineCode",{parentName:"p"},"sqlite3_create_function"),"\nis not called before this code runs, a run-time error will ensue.  Just as CQL must assume that declared\ntables really are created, it also assumes that declared function really are created.  This is another case\nof telling the compiler in advance what the situation will be at runtime."),Object(i.b)("p",null,"SQLite allows for many flexible kinds of user defined functions.  CQL doesn't concern itself with the details of the implementation of the function, it only needs the signature so that it can validate calls."),Object(i.b)("p",null,"See ",Object(i.b)("a",Object(a.a)({parentName:"p"},{href:"https://www.sqlite.org/c3ref/create_function.html"}),"Create Or Redefine SQL Functions"),"."),Object(i.b)("h4",{id:"sql-table-valued-functions"},"SQL Table Valued Functions"),Object(i.b)("p",null,'More recent versions of SQLite also include the ability to add table-valued functions to statements in place of actual tables. These functions can use their arguments to create a "virtual table" value for use in place of a table.  For this\nto work, again SQLite must be told of the existence of the table.  There are a series of steps to make this happen\nbeginning with ',Object(i.b)("inlineCode",{parentName:"p"},"sqlite3_create_module"),' which are described in the SQLite documents under "The Virtual Table Mechanism Of SQLite."'),Object(i.b)("p",null,"Once that has been done, a table-valued function can be defined for most object types.  For instance it is possible to\ncreate a table-valued function like so:"),Object(i.b)("pre",null,Object(i.b)("code",Object(a.a)({parentName:"pre"},{className:"language-sql"}),"declare select function dict_contents(dict object not null)\n   (k text not null, v text not null);\n")),Object(i.b)("p",null,"This is just like the previous type of select function but the return type is a table shape.  Once the above has been done you can legally write something like this:"),Object(i.b)("pre",null,Object(i.b)("code",Object(a.a)({parentName:"pre"},{className:"language-sql"}),"create proc read_dict(dict object not null, pattern text)\nbegin\n  if pattern is not null then\n    select k, v from dict_contents(dict) T1 where T1.k LIKE pattern;\n  else\n    select k, v from dict_contents(dict);\n  end if;\nend;\n")),Object(i.b)("p",null,"This construct is very general indeed but the runtime set up for it is much more complicated than scalar functions\nand only more modern versions of SQLite even support it."),Object(i.b)("h3",{id:"notes-on-builtin-functions"},"Notes on Builtin Functions"),Object(i.b)("p",null,"Some of the SQLite builtin functions are hard-coded,  these are the functions that have semantics that are not readily captured with a simple prototype.  Other SQLite functions can be declared wtih ",Object(i.b)("inlineCode",{parentName:"p"},"declare select funtion ...")," and then used."),Object(i.b)("p",null,"CQL's hard-coded builtin list includes:"),Object(i.b)("p",null,Object(i.b)("em",{parentName:"p"},"Aggregate Functions")),Object(i.b)("ul",null,Object(i.b)("li",{parentName:"ul"},"count"),Object(i.b)("li",{parentName:"ul"},"max"),Object(i.b)("li",{parentName:"ul"},"min"),Object(i.b)("li",{parentName:"ul"},"sum"),Object(i.b)("li",{parentName:"ul"},"total"),Object(i.b)("li",{parentName:"ul"},"avg"),Object(i.b)("li",{parentName:"ul"},"average"),Object(i.b)("li",{parentName:"ul"},"group_concat")),Object(i.b)("p",null,Object(i.b)("em",{parentName:"p"},"Scalar Functions")),Object(i.b)("ul",null,Object(i.b)("li",{parentName:"ul"},"ifnull"),Object(i.b)("li",{parentName:"ul"},"nullif"),Object(i.b)("li",{parentName:"ul"},"upper"),Object(i.b)("li",{parentName:"ul"},"char"),Object(i.b)("li",{parentName:"ul"},"abs"),Object(i.b)("li",{parentName:"ul"},"instr"),Object(i.b)("li",{parentName:"ul"},"coalesce"),Object(i.b)("li",{parentName:"ul"},"last_insert_rowid"),Object(i.b)("li",{parentName:"ul"},"printf"),Object(i.b)("li",{parentName:"ul"},"strftime"),Object(i.b)("li",{parentName:"ul"},"date"),Object(i.b)("li",{parentName:"ul"},"time"),Object(i.b)("li",{parentName:"ul"},"datetime"),Object(i.b)("li",{parentName:"ul"},"julianday"),Object(i.b)("li",{parentName:"ul"},"substr"),Object(i.b)("li",{parentName:"ul"},"trim"),Object(i.b)("li",{parentName:"ul"},"ltrim"),Object(i.b)("li",{parentName:"ul"},"rtrim")),Object(i.b)("p",null,Object(i.b)("em",{parentName:"p"},"Window Functions")),Object(i.b)("ul",null,Object(i.b)("li",{parentName:"ul"},"row_number"),Object(i.b)("li",{parentName:"ul"},"rank"),Object(i.b)("li",{parentName:"ul"},"dense_rank"),Object(i.b)("li",{parentName:"ul"},"percent_rank"),Object(i.b)("li",{parentName:"ul"},"cume_dist"),Object(i.b)("li",{parentName:"ul"},"ntile"),Object(i.b)("li",{parentName:"ul"},"lag"),Object(i.b)("li",{parentName:"ul"},"lead"),Object(i.b)("li",{parentName:"ul"},"first_value"),Object(i.b)("li",{parentName:"ul"},"last_value"),Object(i.b)("li",{parentName:"ul"},"nth_value")),Object(i.b)("p",null,"Special Functions"),Object(i.b)("ul",null,Object(i.b)("li",{parentName:"ul"},"nullable"),Object(i.b)("li",{parentName:"ul"},"ptr")),Object(i.b)("p",null,Object(i.b)("inlineCode",{parentName:"p"},"Nullable")," casts an operand to the nullable version of its type and otherwise does nothing.  This cast might be useful if you need an exact type match in a situation.  It is stripped from any generated SQL and generated C so it has no runtime effect at all other than the indirect consequences of changing the storage class of its operand."),Object(i.b)("p",null,Object(i.b)("inlineCode",{parentName:"p"},"Ptr")," is used to cause a reference type variable to be bound as a long integer to SQLite. This is a way of giving object pointers to SQLite UDFs."))}u.isMDXComponent=!0}}]);