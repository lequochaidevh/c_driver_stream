#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

std::string s = "abc";  // s is lvalue
int a = 10;  // a is lvalue

// rvalue = xvalue + prvalue
// prvalue is TEMPORARY
/*
*@brief
prvalue (pure rvalue) = hầu hết temporary
*   5           // temporary int
*   5 + 3       // temporary int
*   std::string{"abc"}   // temporary string
*   vector<int>{1,2,3}   // temporary vector
*   makeVec()  // if func return by-value
*/

/*
* XVALUE (eXpiring value)
* xvalue ~> rvalue but can get adress
*/

std::string s = "hello";

std::string&& r = std::move(s);  
// r is xvalue (move reference)

std::string t = std::string{"hi"} + s;  
// (std::string{"hi"} + s) is xvalue — result of operator+
// * xvalue is rvalue but have diffirent temporary: it have identity.

//glvalue = lvalue + xvalue
//rvalue = prvalue + xvalue

// CAN GET BY & ? -> xvalue,lvalue,glvalue

/**
 * func(10);                  // 10 là prvalue
 * func(std::string{"hi"});   // temporary string
 * func(getVector());         // getVector() trả prvalue
 */

std::string s = "hello";
// func(std::move(s));        // xvalue
/** ALL
 * void f(const std::string&); // nhận mọi thứ: lvalue + temporary
 * void g(std::string&&);      // chỉ nhận rvalue (temporary/xvalue)
 * Temporary = prvalue
 * rvalue = (temporary) prvalue + (expiring) xvalue
 * lvalue = giá trị có tên, tồn tại lâu
 * xvalue = đối tượng thật nhưng sắp "chết" (khi move)
 */