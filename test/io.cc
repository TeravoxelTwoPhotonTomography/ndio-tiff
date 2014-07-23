/** \file
    Testing reading of nD volumes from file tiff.
    @cond TEST_F

    \todo APPEND test
*/

// solves a std::tuple problem in vs2012
#define GTEST_HAS_TR1_TUPLE     0
#define GTEST_USE_OWN_TR1_TUPLE 1

#include <gtest/gtest.h>
#include "config.h"
#include "nd.h"
#include "helpers.h"

#define countof(e) (sizeof(e)/sizeof(*e))

static
struct _files_t
{ const char  *path;
  nd_type_id_t type;
  size_t       ndim;
  size_t       shape[5];
}

file_table[] =
{ // Set a: Should be i16, but is read by mylib as u16
  {NDIO_TIFF_TEST_DATA_PATH"/test1.tif" ,nd_i16,2,{333,222,1 ,1,1}},
  {NDIO_TIFF_TEST_DATA_PATH"/test2.tif",nd_f32,3,{10 ,20 ,30,1,1}},
  {0}
};

struct tiff:public testing::Test
{
  void SetUp()
  { ndioAddPluginPath(NDIO_BUILD_ROOT);
  }
};

TEST_F(tiff,OpenClose)
{ struct _files_t *cur;
  // Examples that should fail to open
#if 1
  EXPECT_EQ(NULL,ndioOpen("does_not_exist.im.super.serious",ndioFormat("tiff"),"r"));
  EXPECT_EQ(NULL,ndioOpen("",ndioFormat("tiff"),"r"));
  EXPECT_EQ(NULL,ndioOpen("",ndioFormat("tiff"),"w"));
  EXPECT_EQ(NULL,ndioOpen(NULL,ndioFormat("tiff"),"r"));
  EXPECT_EQ(NULL,ndioOpen(NULL,ndioFormat("tiff"),"w"));
#endif
  // Examples that should open
  for(cur=file_table;cur->path!=NULL;++cur)
  { ndio_t file=0;
    EXPECT_NE((void*)NULL,file=ndioOpen(cur->path,ndioFormat("tiff"),"r"));
    EXPECT_STREQ("tiff",ndioFormatName(file));
    ndioClose(file);
  }
}

TEST_F(tiff,Shape)
{ struct _files_t *cur;
  for(cur=file_table;cur->path!=NULL;++cur)
  { ndio_t file=0;
    nd_t form;
    EXPECT_NE((void*)NULL,file=ndioOpen(cur->path,ndioFormat("tiff"),"r"))<<cur->path;
    ASSERT_NE((void*)NULL,form=ndioShape(file))<<ndioError(file)<<"\n\t"<<cur->path;
    EXPECT_EQ(cur->type,ndtype(form))<<cur->path;
    EXPECT_EQ(cur->ndim,ndndim(form))<<cur->path;
    for(size_t i=0;i<cur->ndim;++i)
      EXPECT_EQ(cur->shape[i],ndshape(form)[i])<<cur->path;
    EXPECT_EQ(NULL,nderror(form));
    ndfree(form);
    ndioClose(file);
  }
}

TEST_F(tiff,Read)
{ struct _files_t *cur;
  for(cur=file_table;cur->path!=NULL;++cur)
  { ndio_t file=0;
    nd_t vol;
    EXPECT_NE((void*)NULL,file=ndioOpen(cur->path,ndioFormat("tiff"),"r"));
    ASSERT_NE((void*)NULL, vol=ndioShape(file))<<ndioError(file)<<"\n\t"<<cur->path;
    EXPECT_EQ(vol,ndref(vol,malloc(ndnbytes(vol)),nd_heap));
    EXPECT_EQ(file,ndioRead(file,vol));
    ndfree(vol);
    ndioClose(file);
  }
}

TEST_F(tiff,ReadSubarray)
{ struct _files_t *cur;
  for(cur=file_table;cur->path!=NULL;++cur)
  { ndio_t file=0;
    nd_t vol;
    size_t n;
    size_t pos[32]={0}; // assume the file has less than 32 dimensions
    EXPECT_NE((void*)NULL,file=ndioOpen(cur->path,ndioFormat("tiff"),"r"));
    EXPECT_NE((void*)NULL, vol=ndioShape(file))<<ndioError(file)<<"\n\t"<<cur->path;
    ASSERT_GT(countof(pos),ndndim(vol));
    // setup to read center hypercube
    for(size_t i=0;i<ndndim(vol);++i)
      ndShapeSet(vol,i,ndshape(vol)[i]/2);
    EXPECT_EQ(vol,ndref(vol,malloc(ndnbytes(vol)),nd_heap));
    for(size_t i=0;i<ndndim(vol);++i)
      pos[i]=ndshape(vol)[i]/2;
    EXPECT_EQ(file,ndioReadSubarray(file,vol,pos,NULL));
    ndfree(vol);
    ndioClose(file);
  }
}

typedef ::testing::Types<
#if 0
  uint8_t
#else
  uint8_t,uint16_t,uint32_t,//uint64_t,
   int8_t, int16_t, int32_t,// int64_t,
  float//, double
#endif
  > BasicTypes;

template<class T>
class tiff_Typed:public ::testing::Test
{
public:
  nd_t a;
  tiff_Typed() :a(0) {}
  void SetUp()
  { size_t shape[]={134,513,52,34};
    ndioAddPluginPath(NDIO_BUILD_ROOT);
    EXPECT_NE((void*)NULL,ndreshape(cast<T>(a=ndinit()),countof(shape),shape))<<nderror(a);
    EXPECT_NE((void*)NULL,ndref(a,malloc(ndnbytes(a)),nd_heap))<<nderror(a);
  }
  void TearDown()
  { ndfree(a);
  }
};

TYPED_TEST_CASE(tiff_Typed,BasicTypes);
TYPED_TEST(tiff_Typed,Write)
{ nd_t vol;
  ndio_t file;
  ndio_t fin;
  size_t shape[3]={134,513,52*34}; //last dims should've been cat'd
  EXPECT_NE((void*)NULL,ndioWrite(file=ndioOpen("testout.tif",NULL,"w"),this->a));
  ndioClose(file);
  EXPECT_NE((void*)NULL,fin=ndioOpen("testout.tif",NULL,"r"));
  ASSERT_NE((void*)NULL,vol=ndioShape(fin))<<ndioError(fin)<<"\n\t"<<"testout.tif";
  ndioClose(fin);
  { int i;
    EXPECT_EQ(-1,i=firstdiff(countof(shape),shape,ndshape(vol)))
        << "\texpected shape["<<i<<"]: "<< shape[i] << "\n"
        << "\tread shape["<<i<<"]: "<< ndshape(vol)[i] << "\n";
  }
}
/// @endcond
