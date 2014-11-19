#include "reflect.h"

struct HexaAnnotation {int a, b;};

struct S{
    MEMBER(unsigned, timePosix);
    MEMBER(int, year);
    MEMBER(int, month);
    MEMBER(int, day);
    MEMBER(int, hour);
    MEMBER(int, minute);
    MEMBER(int, second);
    MEMBER(int, fixStatus);
    MEMBER(double, lon);
    MEMBER(double, lat);
    MEMBER(int, alt);
    MEMBER(double, pdop);
    MEMBER(double, hdop);
};

#define Args_1 \
    ESC(template<class MemberInfo, class T1>\
    bool process(T1& a))
#define Args_2 \
    ESC(template<class MemberInfo, class T1, class T2>\
    bool process(T1& a, T2& b))
#define Args_3 \
    ESC(template<class MemberInfo, class T1, class T2, class T3>\
    bool process(T1& a, T2& b, T3& c))
#define SavedArgs_0 (){}
#define SavedArgs_1(Arg1Type, Arg1Name)\
    ESC((Arg1Type& Arg1Name##_)\
    : Arg1Name(Arg1Name##_)\
    {}\
    Arg1Type Arg1Name;)
#define SavedArgs_2(Arg1Type, Arg1Name, Arg2Type, Arg2Name) \
    ESC((Arg1Type& Arg1Name##_, Arg2Type& Arg2Name##_)\
    : Arg1Name(Arg1Name##_)\
    , Arg2Name(Arg2Name##_)\
    {}\
    Arg1Type Arg1Name;\
    Arg2Type Arg2Name;)
#define SavedArgs_3(Arg1Type, Arg1Name, Arg2Type, Arg2Name, Arg3Type, Arg3Name) \
    ESC((Arg1Type& Arg1Name##_, Arg2Type& Arg2Name##_, Arg3Type& Arg3Name##_)\
    : Arg1Name(Arg1Name##_)\
    , Arg2Name(Arg2Name##_)\
    , Arg3Name(Arg3Name##_)\
    {}\
    Arg1Type Arg1Name;\
    Arg2Type Arg2Name;\
    Arg3Type Arg3Name;)

#define DefFun(FunName, SavedArgs, Args, code) \
    namespace\
    {\
        struct FunName\
        {\
            FunName SavedArgs\
            Args\
            {\
                code;\
                return true;\
            }\
        };\
    }

DefFun(EqMul2, SavedArgs_0, Args_2, a = b*2);

DefFun(Interpolate, SavedArgs_1(const double, ratio), Args_3,
       a = b*(1-ratio) + c*(ratio));

struct A{
    MEMBER_EX(double, a, ANNOTATION(HexaAnnotation, ={10,20}));
    MEMBER(double, b);
    MEMBER(double, c);
};

struct NewPrinter {
    template<class MemberInfo, class MemberType>
    bool process(const MemberType& value)
    {
        if(MemberInfo::template Annotation<HexaAnnotation>::has)
        {
            std::cout << std::hex;
            HexaAnnotation a = MemberInfo::template Annotation<HexaAnnotation>::get();
            std::cout << "a.a: " << a.a <<std::endl;
        }
        else
        {
            std::cout << std::dec;
        }
        return std::cout << MemberInfo::name() << ": " << value << std::endl;
    }
};

struct B
{
    MEMBER(int,x);
    MEMBER(int,y);
    MEMBER(A,z);
};

struct C
{
    int a;
    struct TT;
    struct TT;
};

struct RecPrinter {
    template<class MemberInfo, class MemberType>
    enable_if<!IS_REFLECTED(MemberType),bool> process(const MemberType& value)
    {
        return std::cout << MemberInfo::name() << ": " << value << std::endl;
    }

    template<class MemberInfo, class MemberType>
    enable_if<IS_REFLECTED(MemberType),bool> process(const MemberType& value)
    {
        return std::cout << "sub struct" << std::endl;
    }

    template<class MemberInfo>
    bool process(int value)
    {
        return std::cout << "int lol" << std::endl;
    }
};

int main()
{
    B bb;
    RecPrinter rec_print;
    reflect::for_each_member(bb, rec_print);

    A a1 = {1,2,3};
    A a2 = {10,10,10};
    A a3;

    NewPrinter new_print;
    reflect::for_each_member(a2, new_print);

    Interpolate inter(0.2);
    reflect::for_each_member(a3, a1, a2, inter);
    reflect::Printer print;
    reflect::for_each_member(a3, print);
    C cc;
   // reflect::for_each_member(cc, print);
    S a, b;

    reflect::Reader read;
    EqMul2 eqMul2;
    reflect::Printer2 print2;

    reflect::for_each_member(a, read);
    reflect::for_each_member(b, (const S&)a, eqMul2);

    std::cout << std::endl;
    reflect::for_each_member<S::reflect_member_id_year, S::reflect_member_id_second>(a, b, print2);
}
