/*
 GEODIFF - MIT License
 Copyright (C) 2019 Peter Petrik
*/

#include "geodiff.h"
#include "geodiff_testutils.hpp"
#include "gtest/gtest.h"

TEST( ConcurrentCommitsSqlite3Test, test_2_inserts )
{
  std::cout << "geopackage 2 concurent modifications (base) -> (A) and (base) -> (B)" << std::endl;

  std::string base = testdir() + "/base.gpkg";
  std::string modifiedA = testdir() + "/" + "inserted_1_A.gpkg";
  std::string modifiedB = testdir() + "/" + "inserted_1_B.gpkg";
  std::string changesetbaseA = tmpdir() + "/changeset_base_to_A.bin";
  std::string changesetAB = tmpdir() + "/changeset_A_to_B.bin";
  std::string changesetBbase = tmpdir() + "/changeset_B_to_base.bin";
  std::string patchedAB = tmpdir() + "/patched_AB.gpkg" ;

  // create changeset base to A
  ASSERT_EQ( createChangeset( base.c_str(), modifiedA.c_str(), changesetbaseA.c_str() ), GEODIFF_SUCCESS );
  int nchanges;
  ASSERT_EQ( listChanges( changesetbaseA.c_str(), &nchanges ), GEODIFF_SUCCESS );
  ASSERT_EQ( nchanges, 2 * 1 + 3 ); // 3 updates in total and 2 inserts for each feature

  // create changeset A to B
  ASSERT_EQ( createRebasedChangeset( base.c_str(), modifiedB.c_str(), changesetbaseA.c_str(), changesetAB.c_str() ), GEODIFF_SUCCESS );
  ASSERT_EQ( listChanges( changesetAB.c_str(), &nchanges ), GEODIFF_SUCCESS );
  ASSERT_EQ( nchanges, 2 * 1 + 3 ); // 3 updates in total and 2 inserts for each feature

  // apply changeset to A to get AB
  ASSERT_EQ( applyChangeset( modifiedA.c_str(), patchedAB.c_str(), changesetAB.c_str() ), GEODIFF_SUCCESS );

  // check that then new data has both features
  ASSERT_EQ( createChangeset( base.c_str(), patchedAB.c_str(), changesetBbase.c_str() ), GEODIFF_SUCCESS );
  ASSERT_EQ( listChanges( changesetBbase.c_str(), &nchanges ), GEODIFF_SUCCESS );
  ASSERT_EQ( nchanges, 2 * 2 + 3 ); // 3 updates in total and 2 inserts for each feature
}

int main( int argc, char **argv )
{
  testing::InitGoogleTest( &argc, argv );
  init_test();
  int ret =  RUN_ALL_TESTS();
  finalize_test();
  return ret;
}