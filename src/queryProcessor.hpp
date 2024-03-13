#ifndef __QUERY_PROCESSOR_H_
#define __QUERY_PROCESSOR_H_

#include "bufferManager.hpp"
#include "database.hpp"

#define LRU 1
#define MRU 2
#define CLOCK 3
#define PAGE_SIZE 4096

class QueryProcessor
{
public:
  QueryProcessor(int numFrames, int replacementPolicy);

  /**
   * @brief process a select query
   * SELECT * FROM fp.table WHERE col = value
   *
   * @param fp
   * @param col
   * @param value
   */
  void processSelectQuery(FILE* fp, int col, std::string value);

  /**
   * @brief process a join query
   * SELECT * FROM fp1.table JOIN fp2.table ON fp1.table.col1 = fp2.table.col2
   *
   * @param fp1
   * @param fp2
   * @param col1
   * @param col2
   */
  void processJoinQuery(FILE* fp1, FILE* fp2, int col1, int col2);

private:
  ReplacementPolicy* bufferManager;
};

#endif