#include "database.hpp"
#include <iostream>
#include <cstring>
#include <sys/stat.h>
#define PAGE_SIZE 4096

int main()
{

  mkdir("db", S_IRWXU);
 
  FILE* fp;
  fp = fopen("./db/patient.bin", "wb");

  Patient p[1000];
  for (int i = 0; i < 1000; i++)
  {
    std::string name = "abc" + std::to_string(i);
    strcpy(p[i].name, name.c_str());
    p[i].age = rand() % 20 + 5;
    p[i].weight = rand() % 50 + 20;
    std::cout << p[i].name << " " << p[i].age << " " << p[i].weight << std::endl;
  }

  int recordSize = sizeof(Patient);

  // add first 4 bytes in page 1 as number of records in last page

  int i = 0;
  int totalRecords = 1000;
  while (1)
  {
    int numLeft = PAGE_SIZE;
    // starting of each page add number of records in this page
    int possible = (PAGE_SIZE - 4) / recordSize;
    int recordsInPage = std::min(possible, totalRecords);
    fwrite(&recordsInPage, sizeof(int), 1, fp);
    totalRecords -= recordsInPage;
    numLeft -= sizeof(int);

    while (numLeft >= recordSize)
    {
      fwrite(&p[i], recordSize, 1, fp);
      numLeft -= recordSize;
      i++;
      if (i == 1000)
      {
        break;
      }
    }
    while (numLeft > 0)
    {
      char c = '\0';
      fwrite(&c, sizeof(char), 1, fp);
      numLeft--;
    }
    if (i == 1000)
    {
      break;
    }
  }

  fclose(fp);

  fp = fopen("./db/medicalRecord.bin", "wb");

  MedicalRecord m[1000];
  char disease[20][20] = {"covid",    "dengue",    "malaria",       "typhoid",   "chickenpox",
                          "measles",  "cholera",   "hepatitis",     "influenza", "tuberculosis",
                          "smallpox", "polio",     "rabies",        "tetanus",   "yellowfever",
                          "mumps",    "pertussis", "whoopingcough", "pneumonia", "diarrhea"};
  char treatment[20][20] = {"paracetamol", "ibuprofen",   "acetaminophen", "aspirin",    "naproxen",
                            "diclofenac",  "ibupzon",     "ibuprex",       "ibuprin",    "ibuprime",
                            "ibupros",     "ibuprox",     "ibuproxen",     "ibuproxin",  "ibuproxyn",
                            "ibuprozin",   "ibuprozinum", "ibuprozol",     "ibuprozole", "ibuprozolone"};

  for (int i = 0; i < 1000; i++)
  {
    int random = rand() % 1000;
    std::string name = "abc" + std::to_string(random);
    strcpy(m[i].name, name.c_str());
    m[i].vaccinated = rand() % 2;
    strcpy(m[i].disease, disease[rand() % 20]);
    strcpy(m[i].treatment, treatment[rand() % 20]);
    // assign random date
    int day = rand() % 30 + 1;
    int month = rand() % 12 + 1;
    int year = rand() % 20 + 2000;
    std::string date = std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    strcpy(m[i].date, date.c_str());
    std::cout << m[i].name << " " << m[i].vaccinated << " " << m[i].disease << " " << m[i].treatment << " " << m[i].date
              << std::endl;
  }

  recordSize = sizeof(MedicalRecord);

  // add first 4 bytes in page 1 as number of records in last page

  i = 0;
  totalRecords = 1000;
  while (1)
  {
    int numLeft = PAGE_SIZE;
    // starting of each page add number of records in this page
    int possible = (PAGE_SIZE - 4) / recordSize;
    int recordsInPage = std::min(possible, totalRecords);
    fwrite(&recordsInPage, sizeof(int), 1, fp);
    totalRecords -= recordsInPage;
    numLeft -= sizeof(int);

    while (numLeft >= recordSize)
    {
      fwrite(&m[i], recordSize, 1, fp);
      numLeft -= recordSize;
      i++;
      if (i == 1000)
      {
        break;
      }
    }
    while (numLeft > 0)
    {
      char c = '\0';
      fwrite(&c, sizeof(char), 1, fp);
      numLeft--;
    }
    if (i == 1000)
    {
      break;
    }
  }

  fclose(fp);
  return 0;
}
