#pragma once

#include <functional>
#include <string>
#include <vector>
#include <iostream>

namespace test {
    
    class Test{
    public:
        Test(){}
        virtual ~Test(){}

        virtual void OnUpdate(float deltaTime){}
        virtual void OnRender(){}
        virtual void OnImGuiRender(){}
    };

    class TestMenu : public Test{
    public:
        TestMenu(Test*& currentTestPointer);
        virtual ~TestMenu();

        virtual void OnImGuiRender() override;

        template<typename T>
        void RegisterTest(const std::string& name){
            std::cout << "Registering test " << name << std::endl;
            m_Tests.push_back(std::make_pair(name, [](){ return new T(); })); // This is a lambda function that returns a new instance of the test
        }   
    private:
        Test*& m_CurrentTest;
        std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
    };

}