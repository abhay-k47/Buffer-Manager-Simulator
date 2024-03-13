#ifndef __BUF_MANAGER_H_
#define __BUF_MANAGER_H_

#include <iostream>
#include <iterator>
#include <list>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>
#define PAGE_SIZE 4096

/**
 * @brief an entity in Memory to store a Page
 *
 */
class Frame
{
public:
  Frame() = default;
  Frame(const Frame& f);
  ~Frame();
  friend class LRUBufferManager;
  friend class ClockBufferManager;
  friend class MRUBufferManager;

private:
  int pageNum;  // page number of page
  char* pageData;  // data in page
  FILE* fp;  // file to which this page belongs to
  bool pinned;  // either pinned or unpinned
  bool second_chance;  // for clock replacement algorithm

  /**
   * @brief populates a frame in Memory
   *
   * @param fp
   * @param pageNum
   * @param pageData
   * @param pinned
   */
  void setFrame(FILE* fp, int pageNum, char* pageData, bool pinned);

  /**
   * @brief unpin a frame
   *
   */
  void unpinFrame();
};

struct PairHash
{
  size_t operator()(const std::pair<FILE*, int>& p) const
  {
    size_t h1 = std::hash<FILE*>()(p.first);
    size_t h2 = std::hash<int>()(p.second);
    return h1 ^ h2;
  }
};

class BufStats
{
public:
  int accesses{0};
  int diskreads{0};
  int pageHits{0};

  /**
   * @brief clear stats
   *
   */
  void clear();
};

class ReplacementPolicy
{
public:
  virtual ~ReplacementPolicy() = default;

  /**
   * @brief get stats
   *
   * @return BufStats
   */
  virtual BufStats getStats() const = 0;

  /**
   * @brief clear stats
   *
   */
  virtual void clearStats() = 0;

  /**
   * @brief get a page from buffer
   *
   * @param fp file pointer
   * @param pageNum page number
   * @return char*
   */
  virtual char* getPage(FILE* fp, int pageNum) = 0;

  /**
   * @brief unpin a page
   *
   * @param fp file pointer
   * @param pageNum page number
   */
  virtual void unpinPage(FILE* fp, int pageNum) = 0;
};

/**
 * @brief implements LRU replacement algorithm
 *
 */
class LRUBufferManager : public ReplacementPolicy
{
public:
  LRUBufferManager(int numFrames);
  ~LRUBufferManager();
  BufStats getStats() const override;
  void clearStats() override;
  char* getPage(FILE* fp, int pageNum) override;
  void unpinPage(FILE* fp, int pageNum) override;

private:
  int numFrames;  // number of frames that can be fit in pool
  std::list<Frame> lru;  // list to implement LRU
  std::unordered_map<std::pair<FILE*, int>, std::list<Frame>::iterator, PairHash>
      mp;  // map to identify whether a page is present in buffer or not
  BufStats stats;
  FILE* file;  // file to store log
};

/**
 * @brief implements clock replacement algorithm
 *
 */
class ClockBufferManager : public ReplacementPolicy
{
public:
  ClockBufferManager(int numFrames);
  ~ClockBufferManager();
  BufStats getStats() const override;
  void clearStats() override;
  char* getPage(FILE* fp, int pageNum) override;
  void unpinPage(FILE* fp, int pageNum) override;

private:
  int numFrames;  // number of frames that can be fit in pool
  int numPages;
  Frame* bufferPool;  // list to implement clock
  int clock_hand;  // clock hand
  BufStats stats;
  FILE* file;  // file to store log
};

/**
 * @brief implements MRU replacement algorithm
 *
 */
class MRUBufferManager : public ReplacementPolicy
{
public:
  MRUBufferManager(int numFrames);
  ~MRUBufferManager();
  BufStats getStats() const override;
  void clearStats() override;
  char* getPage(FILE* fp, int pageNum) override;
  void unpinPage(FILE* fp, int pageNum) override;

private:
  int numFrames;  // number of frames that can be fit in pool
  std::list<Frame> mru;  // list to implement MRU
  std::unordered_map<std::pair<FILE*, int>, std::list<Frame>::iterator, PairHash>
      mp;  // map to identify whether a page is present in buffer or not
  BufStats stats;
  FILE* file;  // file to store log
};

#endif
