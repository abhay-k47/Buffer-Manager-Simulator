#ifndef _DB_HPP
#define _DB_HPP

struct Patient
{
  char name[20];
  int age;
  int weight;
};

struct MedicalRecord
{
  char name[20];
  int vaccinated;
  char disease[20];
  char treatment[20];
  char date[20];
};

#endif