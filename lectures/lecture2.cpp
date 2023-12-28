#include <iostream>
#include <vector>
#include <fstream>

class Student
{
	std::string	m_first	= "John";
	std::string	m_last	= "Doe";
	int			m_id	= 0;
	float		m_gpa	= 0.0f;

public:
	Student() {}

	Student(std::string first, std::string last, int id, float gpa)
		: m_first	(first)
		, m_last	(last)
		, m_id		(id)
		, m_gpa		(gpa)
	{
	}

	int getAvg() const
	{
		return m_gpa;
	}

	int getID() const
	{
		return m_id;
	}

	std::string getName() const
	{
		return m_first + " " + m_last;
	}

	void print() const
	{
		std::cout << m_first << " " << m_last << " ";
		std::cout << m_id << " " << m_gpa << std::endl;
	}
};

class Course
{
	std::string				m_name		= "CS 202";
	std::vector<Student>	m_students;

	public:
		Course() {}

		Course(const std::string& name)
			: m_name(name)
		{
		}

		void addStudent(const Student& s)
		{
			m_students.push_back(s);
		}

		std::vector<Student> getStudents() const
		{
			return m_students;
		}

		void loadFromFile(const std::string& filename)
		{
			std::ifstream fin(filename);
			std::string first, last;
			int id;
			float gpa;

			while (fin >> first)
			{
				fin >> last >> id >> gpa;
				addStudent(Student(first, last, id, gpa));
			}
		}

		void print() const{
			for (const auto& s : m_students)
			{
				s.print();
			}
		}
};

int main(int argc, char * argv[])
{
	Course c("CS 202");
	c.loadFromFile("students.txt");
	c.print();

	return 0;
}
