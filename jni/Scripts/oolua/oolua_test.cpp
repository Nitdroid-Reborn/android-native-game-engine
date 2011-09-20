#include <oolua.h>
#include <oolua_error.h>
#include <iostream>
#include <stdlib.h>

#include <ScriptManager.h>
#include <Script.h>

#include <QDebug>
class boo {
public:
    void hello() {
        std::cout<<"Boo hello";
    }
};

class foo
{
public:
    void testFunc(boo* b) {
        b->hello();
    }


    foo() {
      //  std::cout<<"Constructor "<<x;
    }
};

OOLUA_CLASS_NO_BASES(boo)
    OOLUA_NO_TYPEDEFS
    OOLUA_ONLY_DEFAULT_CONSTRUCTOR
    OOLUA_MEM_FUNC_0(void, hello)
OOLUA_CLASS_END
EXPORT_OOLUA_FUNCTIONS_1_NON_CONST(boo,hello)/*function being exposed*/
EXPORT_OOLUA_FUNCTIONS_0_CONST(boo)


//Define class for lua
OOLUA_CLASS_NO_BASES(foo)//class has no bases
    OOLUA_NO_TYPEDEFS
    OOLUA_CONSTRUCTORS_BEGIN
    OOLUA_CONSTRUCTORS_END
        OOLUA_MEM_FUNC_1(void,testFunc, in_p<boo*>)
OOLUA_CLASS_END

//Export class to lua
EXPORT_OOLUA_FUNCTIONS_1_NON_CONST(foo,testFunc)/*function being exposed*/
EXPORT_OOLUA_FUNCTIONS_0_CONST(foo)


//Run the tests
void test()
{

    ScriptManager manager;
    manager.Initialize();


    OOLUA::register_class<foo>(manager.getState());
    OOLUA::register_class<boo>(manager.getState());

    Script* s1=new Script();

    if(!s1->runString("f = foo:new(); b = boo:new(); f:testFunc(b);"))
        qDebug()<<OOLUA::get_last_error(s1->threadState).c_str();

    delete s1;

    manager.Release();
        //Create script instance
    /*    OOLUA::Script s;

        //Register class
        s.register_class<foo>();
        OOLUA::register_class_static<foo>(s, "getboo", staticMemberFunc);
        s.register_class<boo>();


        lua_State* threadState = lua_newthread(s.get_ptr());




           // save a pointer to the thread manager object in the global table
           // using the new thread's vm pointer as a key
           //lua_pushlightuserdata(s.get_ptr(), threadState);
           //lua_pushlightuserdata(s.get_ptr(), &s );
           //lua_settable(s.get_ptr(), LUA_GLOBALSINDEX );

            std::cout<<lua_gettop(s.get_ptr());

            int n = lua_gettop(s.get_ptr());
            for (int i(1); i<=n; ++i)
                if (lua_isthread(s.get_ptr(), i))		{
                    printf("thread\n");
                }

            lua_pop(s.get_ptr(), 1);
            //lua_remove(s.get_ptr(), 1);
            //lua_State* threadState2 = lua_newthread(s.get_ptr());
            //std::cout<<luaL_typename(s.get_ptr(), -1);
           //

          /*  n = lua_gettop(s.get_ptr());
            for (int i(1); i<=n; ++i)
                if (lua_isthread(s.get_ptr(), i))		{
                    printf("thread\n");
                }
*/

/*           s.gc();

           //lua_pushstring(s.get_ptr(), "hello");
           lua_pushstring(threadState, "hello");
           // s.gc();

           printf("%s\n", luaL_typename(threadState, -1));

*/



}

int main()
{
        test();

        return 0;
}
