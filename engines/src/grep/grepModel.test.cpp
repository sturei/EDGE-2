

#include <gtest/gtest.h>
#include "grep/grepModel.h"
#include "grep/gitem.h"

using namespace e2;

TEST(GRepModelTest, BasicOperations) {
    GRepModel model;
    GPoint firstItem(5);
    GPoint secondItem(4);

    EXPECT_EQ(model.numGItems(), 0);
    model.addGItem(&firstItem);
    EXPECT_EQ(model.numGItems(), 1);
    EXPECT_EQ(model.gItem(0), &firstItem);
    model.addGItem(&secondItem);
    EXPECT_EQ(model.numGItems(), 2);
    
    auto drawlist = model.drawlist();
    EXPECT_EQ(drawlist.size(), 2);
    EXPECT_EQ(drawlist[0], &firstItem);
    EXPECT_EQ(drawlist[1], &secondItem);
    
    EXPECT_EQ(model.gItem(1), &secondItem);
    model.removeGItem(0);
    EXPECT_EQ(model.numGItems(), 1);
    EXPECT_EQ(model.gItem(0), &secondItem);
    model.removeGItem(0);
    EXPECT_EQ(model.numGItems(), 0);
}
