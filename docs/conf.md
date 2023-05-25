# The syntax of Conf in Backus-Naur form

## Lexical definition

```EBNF
(* The syntax of Conf in Backus-Naur Form. *)

program ::= (class_def | func_def | assign_stmt)+

block ::= '{' (compound_statement | statement)* '}'

annotation ::= '@' func_call

(* simple statement*)
(* assignment must precede expression, else parsing a simple assignment
   will throw a syntax error *)
statement ::= assign_stmt ';'
            | expression ';'
            | return_stmt ';'
            | raise_stmt ';'
            | assert_stmt ';'
            | 'break' ';'     (* only for_stmt and match_stmt *)
            | 'continue' ';'  (* only for_stmt and match_stmt *)

(* assignment statement *)
assign_stmt ::= (targets '=') expressions

targets ::= target (',' target)* | annotation identifier
target ::= identifier | attr_ref | slicing

(* expression list *)
expressions ::= expression (',' expression)*

(* expression *)
expression ::= cond_expr

(* Operator precedence and associativity
Operators                   | Type of operation         | Associativity
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
(exprs...)                  | parenthesized expression  |
[exprs...]                  | list                      |
{key: value...}             | dictionary                |
{exprs}                     | set                       |
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
x[index]                    | subscription              | left to right
x[index:index]              | slicing                   | left to right
x(arguments...)             | function call             | left to right
x.attribute                 | attribute reference       | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**                          | exponentiation            | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+x                          | positive                  | left to right
-x                          | negative                  | left to right
~x                          | bitwise NOT               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*                           | multiplication            | left to right
/                           | division                  | left to right
//                          | floor division            | left to right
%                           | remainder                 | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                           | addition                  | left to right
-                           | substraction              | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
<<                          | left shift                | left to right
>>                          | right shift               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
&                           | bitwise AND               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
^                           | bitwise XOR               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
|                           | bitwise OR                | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
in                          | including membership      | left to right
not in                      | not including membership  | left to right
is                          | identity                  | left to right
is not                      | not identity              | left to right
<                           | less than                 | left to right
<=                          | less or equal than        | left to right
>                           | greater than              | left to right
>=                          | greater or equal than     | left to right
==                          | equal                     | left to right
!=                          | not equal                 | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
not                         | boolean NOT               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
and                         | boolean AND               | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
or                          | boolean OR                | left to right
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
if - else                   | conditional expression    |
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
=                           | assignment expression     |
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Note:
  all comparison operators have the same priority, which is lower
than that of arithmetic, shifting or bitwise operation.
  comparison can be chained arbitrarily and have the interpretation
that is conventional in mathematics. like as expression "a < b < c"
is equivalent to "a < b and b < c".
*)

(* conditional expression *)
cond_expr ::= logic_or ['if' logic_or 'else' expression]

(* logical expression *)
logic_or ::= logic_and | logic_or 'or' logic_and
logic_and ::= logic_not logic_and 'and' logic_not
logic_not ::= comp_ops | 'not' logic_not

(* relational expression *)
comp_ops ::= bitwise_or (comp_operator bitwise_or)*
comp_operator ::= '<' | '<=' | '>' | '>=' | '==' | '!='
                | 'is' ['not'] | ['not'] 'in'

(* bitwise operators *)
bitwise_or ::= bitwise_or '|' bitwise_xor | bitwise_xor
bitwise_xor ::= bitwise_xor '^' bitwise_and | bitwise_and
bitwise_and ::= bitwise_and '&' shift_expr | shift_expr

(* shifting expression *)
shift_expr ::= shift_expr ('<<' | '>>') add_expr | add_expr

(* arithmetic operators *)
add_expr ::= add_expr ('+' | '-') mul_expr | mul_expr

mul_operator ::= '*' | '/' | '//' | '%'
mul_expr ::= mul_expr mul_operator unary_expr | unary_expr

unary_expr ::= ('+' | '-' | '~') unary_expr | power_expr

power_expr ::= primary ['**' unary_expr]

(* primary elements *)
primary ::= attr_ref | func_call | slicing | atom

(* attribute reference *)
attr_ref ::= primary '.' identifier

(* function call *)
func_call ::= primary '(' [arg_list] ')'

arg_list ::= arg (',' arg)*
arg ::= identifier | expression

(* slicing *)
slicing ::= primary '[' slice_list ']'

slice_list ::= expression | proper_slice
proper_slice ::= [lower_bound] ':' [upper_bound] [':' [stride]]
lower_bound ::= expression
upper_bound ::= expression
stride ::=  expression

(* atomic expression *)
atom ::= identifier
       | 'True'
       | 'False'
       | 'None'
       | str
       | number
       | group_expr
       | list
       | (dict | set)

(* identifier *)
identifier ::= (alpa | hyphen) (alpa | hyphen | dec_digits)*

hyphen ::= '_'
alpa ::= 'a'..'z' | 'A'..'Z'

(* string literal *)
str ::= "'" utf8* "'" | '"' utf8* '"'

asc ::= \x00..\x7f
u1 ::= \x80..\xbf
u2 ::= \xc2..\xdf u1
u3 ::= \xe0..\xef u2 u1
u4 ::= \xf0..\xf4 u3 u2 u1
utf8 ::= asc | u2 | u3 | u4

(* number literal *)
number ::= integer | float_point

(* integer literal *)
integer ::= bin_int | oct_int | dec_int | hex_int

bin_int ::= '0' ('b' | 'B') bin_digits+
bin_digits ::= '0' | '1'

oct_int ::= '0' ('o' | 'O') oct_digits+
oct_digits ::= '0'..'7'

dec_int ::= nonzero_dec_digits dec_digits*
dec_digits ::= '0'..'9'
nonzero_dec_digits ::= '1'..'9'

hex_int ::= '0' ('x' | 'X') hex_digits+
hex_digits ::= '0'..'9' | 'a'..'f' | 'A'..'F'

(* floating point literal *)
float_point ::= exponent_float | point_float

exponent_float ::= (point_float | dec_int) exponent
exponent ::= ('e' | 'E') ['+' | '-'] dec_int
point_float ::= dec_int '.' dec_digits+

(* group expression *)
group_expr ::= '(' expression ')'

(* list literal *)
list ::= '[' [expressions] ']'
set ::= '{' [expressions] '}'

(* dictionay literal *)
dict ::= '{' [kvs] '}'
kvs ::= kv (',' kv)*
kv ::= expression ':' expression

(* return statement *)
return_stmt ::= 'return' [expression]

(* raise statement *)
raise_stmt ::= 'raise' expression

(* assert statement *)
assert_stmt ::= 'assert' expression [',' expression]

(* compound statement *)
compound_statement ::= func_def
                     | class_def
                     | if_stmt
                     | for_stmt
                     | match_stmt

(* function definition *)
func_def ::= [annotation] 'func' identifier '(' [param_list] ')' block

param_list ::= params [',' v_param] | v_param
v_param ::= '...'
params ::= param (',' param)*
param ::= identifier ['=' expression]

(* class definition *)
class_def ::= 'class' identifier '{' func_def* '}'

(* if-elif-else statement *)
if_stmt ::= 'if' expression block elif_stmt* [else_stmt]

elif_stmt ::= 'elif' expression block
else_stmt ::= 'else' block

(* for statement *)
for_stmt ::= 'for' targets 'in' expressions block

(* match statement *)
match_stmt ::= 'match' expression '{' case_stmt+ [default_stmt] '}'

case_stmt ::= 'case' expression block
default_stmt ::= 'default' block

```

## Conf example

```python
# example for DNS parser

@attribute("protocol")
PROTOCOL = 998

@attribute("assember")
ASSEMBER = None

@attribte("analyzer")
func main(analyzer, upstrm, downstrm, output) {
  hdr = struct.add_fmt(">HHHHHH", {1: "flags", 2: "que_cnt", 3: "ans_cnt"}).unpack(buf)
  if header.flags >> 15 == 1 {
    hosts = []
    for i in range(0, hdr.que_cnt) {
      que = struct.add_fmt(">sHH", {0: "host"}).unpack(buf);
      hosts.add(que.host);
    }

    ips = []
    for i in range(0, hdr.ans_cnt) {
      ans = struct.add_fmt(">HHHIH", {4: "len"}).unpack(buf)
      ip = struct.add_fmt("{}s".format(ans.len), {0: "ip"}).unpack(buf);
      ips.add(ip)
    }

    if not hosts.empty() and not ips.empty() {
      output.action_type = "0000";
      output.entity_type = "0000";
      output.conf_id = "0000000000000000";
      output.app_type = "0000000";
      output.proto_type = "000000000";
      output.domain = ",".join(hosts)
      output.ips = ",".join(ips);
    }
  }
}

```
