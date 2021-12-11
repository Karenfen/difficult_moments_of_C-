#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <memory>

#include "studentsGroup.pb.h"
#include "fullName.pb.h"
#include "student.pb.h"





bool operator==(const Fullname& left, const Fullname& right)
{
    return (left.surname() == right.surname() && left.name() == right.name() && left.patronymic() == right.patronymic());
}

std::ostream& operator<<(std::ostream& out, const Fullname& full_name)
{
    out << full_name.surname() << " " << full_name.name();
    if (full_name.has_patronymic())
        out << " " << full_name.patronymic();

    return out;
}

std::ostream& operator<<(std::ostream& out, const Student& student_to_print)
{
    out << "Full name: " << student_to_print.full_name() << "\n";
    out << "Scors: ";
    std::copy(student_to_print.scores().begin(), student_to_print.scores().end(), std::ostream_iterator<int>(out, " "));
    out << "\nAverage score: " << student_to_print.average_score() << "\n";

    return out;
}

std::ostream& operator<<(std::ostream& out, const students::StudentsGroup& group)
{
    out << "Group\n";
    out << "========================================\n";
    std::copy(group.students().begin(), group.students().end(), std::ostream_iterator<Student>(out, "========================================\n"));

    return out;
}


class IRepository
{
    virtual void Open(const std::string path) = 0;
    virtual void Save(const std::string path) = 0;
};

class IMethods 
{
    virtual double GetAverageScore(const Fullname& name) = 0;
    virtual std::string GetAllInfo(const Fullname& name) = 0;
    virtual std::string GetAllInfo() = 0;
};


class StudentsGroup : public IRepository, public IMethods
{
public:
    StudentsGroup()
    {
        STGR = std::make_unique<students::StudentsGroup>();
    }

    ~StudentsGroup()
    {
        STGR = nullptr;
    }

    virtual void Open(const std::string path)
    {
        std::ifstream in(path, std::ios_base::binary);

        if (STGR->ParseFromIstream(&in))
            std::cout << "success in" << std::endl;
        else
            std::cout << "Error! in" << std::endl;

        in.close();
    }

    virtual void Save(const std::string path)
    {
        std::ofstream out(path, std::ios_base::binary);

        if (STGR->SerializeToOstream(&out))
            std::cout << "success out" << std::endl;
        else
            std::cout << "Error! out" << std::endl;

        out.close();
    }

    virtual double GetAverageScore(const Fullname& name)
    {
        auto student = std::find_if(STGR->students().begin(), STGR->students().end(), [&](const Student& person) {
            return (person.full_name() == name); } );

        if (student == STGR->students().end())
            std::cout << "studen not found!\n";
        else
            return student->average_score();
    }

    virtual std::string GetAllInfo(const Fullname& name)
    {
        auto student = std::find_if(STGR->students().begin(), STGR->students().end(), [&](const Student& person) {
            return (person.full_name() == name); });

        std::stringstream result;

        if (student == STGR->students().end())
            result << "studen not found!\n";
        else
        {
            
            result << "Full name: " << student->full_name().surname() << " " << student->full_name().name();

            if (student->full_name().has_patronymic())
            {
                result << " ";
                result << student->full_name().patronymic();
            }

            result << "\nScores: ";

            for (const auto& element : student->scores())
            {
                result << element << " ";
            }

            result << "\nAverage score: ";
            result << student->average_score();
        }

        return result.str();
    }

    virtual std::string GetAllInfo()
    {
        std::stringstream result;

        result << "Group:\n";

        for (const auto& element : STGR->students())
        {
            result << GetAllInfo(element.full_name()) << "\n";
        }

        return result.str();
    }

private:
    std::unique_ptr<students::StudentsGroup> STGR{ nullptr };
};





int main()
{

    students::StudentsGroup* group = new students::StudentsGroup;


    Fullname* stud2_name = new Fullname;

    stud2_name->set_name("Avelina");
    stud2_name->set_surname("Yukhalova");
    stud2_name->set_patronymic("Evgenevna");


    Student* stud2 = new Student;

    stud2->set_allocated_full_name(stud2_name);
    stud2->add_scores(4);
    stud2->add_scores(4);
    stud2->add_scores(5);
    stud2->set_average_score(4.5);

    Fullname* stud1_name = new Fullname;

    stud1_name->set_name("Ilia");
    stud1_name->set_surname("Kiselev");
    stud1_name->set_patronymic("Aleksandrovich");


    Student* stud1 = new Student;

    stud1->set_allocated_full_name(stud1_name);
    stud1->add_scores(3);
    stud1->add_scores(4);
    stud1->add_scores(3);
    stud1->set_average_score(3.3);


    group->add_students()->CopyFrom(*stud1);
    group->add_students()->CopyFrom(*stud2);
    

    std::ofstream out("C:/Users/mybut/Desktop/StudentsGroup.bin", std::ios_base::binary);

    if (group->SerializeToOstream(&out))
        std::cout << "success out" << std::endl;
    else
        std::cout << "Error! out" << std::endl;

    out.close();


    StudentsGroup group2;
    group2.Open("C:/Users/mybut/Desktop/StudentsGroup.bin");


    Fullname name2;
    name2.set_name("Ilia");
    name2.set_surname("Ivanov");
   
    std::cout << group2.GetAverageScore(*stud1_name) << std::endl;
    std::cout << group2.GetAverageScore(name2) << std::endl;
    std::cout << group2.GetAllInfo(*stud1_name) << std::endl;
    std::cout << group2.GetAllInfo(name2) << std::endl;

    std::cout << group2.GetAllInfo();

    group2.Save("C:/Users/mybut/Desktop/StudentsGroup2.bin");
    

}

