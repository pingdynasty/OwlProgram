#ifndef __RESOURCE_STORAGE_H__
#define __RESOURCE_STORAGE_H__

#include <cstddef>
#include "FloatArray.h"

class Resource {
public:
  /**
   * Check if data is available
   */
  bool hasData() const {
    return data != NULL;
  }

  /**
   * Get pointer to data. This may be NULL if no buffer is assigned yet.
   */
  void* getData() {
    return data;
  }

  /**
   * Get buffer size in bytes
   */
  size_t getSize() const {
    return size;
  }    

  bool exists() const {
    return size != 0;
  }

  /**
   * Get resource name
   */
  const char* getName() const {
    return name;
  }

  /**
   * Array conversion.
   *
   * @param offset: offset in bytes
   * @param max_size maximum size, actual size can be smaller depending on object size
   */
  template<typename Array, typename Element>
  Array asArray(size_t offset = 0, size_t max_size = 0xFFFFFFFF);

  FloatArray asFloatArray(size_t offset = 0, size_t max_size = 0xFFFFFFFF){
    return asArray<FloatArray, float>(offset, max_size);
  }
  
  /**
   * Get resource from storage.
   * 
   * @param name resource name
   * 
   * @return NULL if resource does not exist or can't be read.
   * 
   */
  static Resource open(const char* name);

  /**
   * Open resource and load data.
   * Allocates extra memory if required, which will be garbage collected 
   * in the object destructor.
   */
  static Resource load(const char* name);

  /**
   * Read data from resource into memory
   *
   * @param len maximum number of bytes to read
   * @param offset index of first byte to read from
   *
   * @return number of bytes actually read
   */
  size_t read(void* dest, size_t len, size_t offset=0); 

  Resource(): name(NULL), size(0), data(NULL), allocated(false) {}
  ~Resource();
protected:
  Resource(const char* name, size_t size, void* data)
    : name(name), size(size), data(data) {}
  const char* name;
  size_t size;
  void* data;
  bool allocated;
};
#endif
