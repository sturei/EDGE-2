#include "gtest/gtest.h"
#include "brep/brepModel.h"

using namespace e2;

TEST(BRepModelTest, DefaultConstructor) {
    BRepModel model;
    EXPECT_EQ(model.bodies().size(), 0);        
};

TEST(BRepModelTest, AddBody) {
    BRepModel model;
    Body body1;
    Body body2;
    size_t index1 = model.addBody(body1);
    size_t index2 = model.addBody(body2);
    EXPECT_EQ(model.numBodies(), 2);        
    EXPECT_EQ(index1, 0);
    EXPECT_EQ(index2, 1);
    // The BRepModel owns its own copies of the bodies, hence the addresses should be different. 
    EXPECT_NE(&body1, &model.bodies()[0]);
    EXPECT_NE(&body2, &model.bodies()[1]);
    // The body accessor should be consistent with the vector accessor
    EXPECT_EQ(model.numBodies(), model.bodies().size());
    EXPECT_EQ(&model.body(0), &model.bodies()[0]);
    EXPECT_EQ(&model.body(1), &model.bodies()[1]);
}

TEST(BRepModelTest, RemoveBody) {
    BRepModel model;
    Body body1;
    Body body2;
    Body body3;
    model.addBody(body1);
    model.addBody(body2);
    model.addBody(body3);
    EXPECT_EQ(model.bodies().size(), 3);        
    model.removeBody(1);
    EXPECT_EQ(model.bodies().size(), 2);        
};

TEST(BRepModelTest, StreamOutputOperator) {
    BRepModel model;
    Body body1;
    Body body2;
    model.addBody(body1);
    model.addBody(body2);

    std::ostringstream oss;
    oss << model;
    std::string modelStr = oss.str();

    //std::cout << "BRepModel output:\n" << modelStr << std::endl; --- IGNORE ---

    // Check that the output string contains some expected substrings
    EXPECT_NE(modelStr.find("BRepModel with 2 bodies."), std::string::npos);
    EXPECT_NE(modelStr.find("Body with 0 cells and 0 cocells."), std::string::npos);
}   


