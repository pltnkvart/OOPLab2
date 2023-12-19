#include <gtest/gtest.h>
#include "../train/train.h"

//CONTAINER TESTS
TEST(ContainerConstuctor, DefaultConstructor) {
    Container container;
    ASSERT_EQ(container.getName(), "");
    ASSERT_EQ(container.getMass(), 0);
    ASSERT_EQ(container.getVolume(), 0);
    ASSERT_EQ(container.getCategory(), EMPTY);
}

TEST(ContainerConstuctor, ParameterizedContainerConstructor) {
    Container container("a", 3.1, 4.2, BULK);
    ASSERT_EQ(container.getName(), "a");
    ASSERT_EQ(container.getMass(), 4.2);
    ASSERT_EQ(container.getVolume(), 3.1);
    ASSERT_EQ(container.getCategory(), BULK);
}

TEST(ContainerConstuctor, DangerousConstructor) {
    Container container(2.3, 4.5);
    ASSERT_EQ(container.getName(), "Undefined");
    ASSERT_EQ(container.getMass(), 2.3);
    ASSERT_EQ(container.getVolume(), 4.5);
    ASSERT_EQ(container.getCategory(), DANGEROUS);
}

TEST(СontainerGettersAndSetters, СontainerGettersAndSetters) {
    Container container;
    container.setMass(2);
    container.setVolume(3);
    container.setName("asd");
    container.setCategory(BULK);
    ASSERT_EQ(container.getMass(), 2);
    ASSERT_EQ(container.getVolume(), 3);
    ASSERT_EQ(container.getName(), "asd");
    ASSERT_EQ(container.getCategory(), BULK);
}

TEST(ContainerGetDensity, ContainerGetDensity) {
    Container container ("Asd", 20.0, 10.0, PIECES);
    ASSERT_EQ(container.getDensity(), 0.5);
}

TEST(ContainerTransferCargo, TransferCargo) {
    Container container1 ("Container1", 20.0, 10.0, PIECES);
    Container container2 ("Container2", 10.0, 5.0, PIECES);
    container1.transferCargo(2.0, container2);
    ASSERT_EQ(container1.getMass(), 8);
    ASSERT_EQ(container2.getMass(), 7);
}

TEST(ContainerTransferCargo, TransferCargoToEmpty) {
    Container container1;
    Container container2 ("Container2", 10.0, 5.0, PIECES);
    container2.transferCargo(2.0, container1);
    ASSERT_EQ(container1.getMass(), 2.0);
    ASSERT_EQ(container2.getCategory(), EMPTY);
}

TEST(ContainerTransferCargo, TransferAllCargo) {
    Container container1 ("Container1", 20.0, 10.4, PIECES);
    Container container2 ("Container2", 10.0, 5.4, PIECES);
    container1>>(container2);
    ASSERT_EQ(container1.getMass(), 0);
    ASSERT_EQ(container2.getMass(), 15.8);
}

TEST(ContainerTransferCargo, ThrowErrorsTransfer) {
    Container container1 ("Container1", 20.0, 10.4, BULK);
    Container container2 ("Container2", 10.0, 5.4, PIECES);
    Container container3 ("Container3", 5.0, 3.0,  BULK);
    Container container4 (3.0, 5.0);
    Container container5 (5.7, 3.4);
    ASSERT_THROW(container1.transferCargo(2.0, container2), std::runtime_error);
    ASSERT_THROW(container3.transferCargo(2.0, container1), std::runtime_error);
    ASSERT_THROW(container4.transferCargo(2.0, container5), std::runtime_error);
}

TEST(ContainerInputOperator, InputOperator) {
    Container container;
    std::istringstream input("Container1 50.0 10.0 1");
    input >> container;
    ASSERT_EQ(container.getName(), "Container1");
    ASSERT_EQ(container.getMass(), 50.0);
    ASSERT_EQ(container.getVolume(), 10.0);
    ASSERT_EQ(static_cast<int>(container.getCategory()), 1);
}

TEST(ContainerOutputOperator, OutputOperator) {
    Container container("Container2", 30.0, 5.0, BULK);
    std::ostringstream output;
    output << container;
    ASSERT_EQ(output.str(), "Container2 Mass - 5 Volume - 30 Category - BULK\n");
}

TEST(ContainerAddCargo, AddCargo) {
    Container container1 ("Container1", 20.0, 10.4, PIECES);
    container1.addCargo(3.3);
    ASSERT_EQ(container1.getMass(), 13.7);
}

// TRAIN TESTS
TEST(TrainConstructor, DefaultConstructor) {
    Train train;
    ASSERT_EQ(train.getMaxWeight(), 0.0);
    ASSERT_EQ(train.getMaxVolume(), 0.0);
    ASSERT_EQ(train.getCount(), 0);
    ASSERT_EQ(train.getContainers(), nullptr);
}

TEST(TrainConstructor, ParameterizedConstructor) {
    Container containersInit[2];
    Train train(2, containersInit, 200.0, 100.0);
    ASSERT_EQ(train.getMaxWeight(), 200.0);
    ASSERT_EQ(train.getMaxVolume(), 100.0);
    ASSERT_EQ(train.getCount(), 2);
}

TEST(TrainConstructor, ConstructorByOneContainer) {
    Container oneContainer("Container1", 50.0, 10.0, GAS);
    Train train(oneContainer, 200.0, 100.0);
    ASSERT_DOUBLE_EQ(train.getMaxWeight(), 200.0);
    ASSERT_DOUBLE_EQ(train.getMaxVolume(), 100.0);
    ASSERT_EQ(train.getCount(), 1);
    ASSERT_EQ(train[0].getName(), "Container1");
    ASSERT_DOUBLE_EQ(train[0].getMass(), 10.0);
    ASSERT_DOUBLE_EQ(train[0].getVolume(), 50.0);
    ASSERT_EQ(train[0].getCategory(), GAS);
}

TEST(TrainInputOperator, InputOperator) {
    Train train;
    std::istringstream input("100.0 200.0 2\nContainer1 50.0 10.0 1\nContainer2 30.0 5.0 2");

    input >> train;

    ASSERT_DOUBLE_EQ(train.getMaxWeight(), 100.0);
    ASSERT_DOUBLE_EQ(train.getMaxVolume(), 200.0);
    ASSERT_EQ(train.getCount(), 2);

    ASSERT_EQ(train[0].getName(), "Container1");
    ASSERT_DOUBLE_EQ(train[0].getMass(), 50.0);
    ASSERT_DOUBLE_EQ(train[0].getVolume(), 10.0);
    ASSERT_EQ(static_cast<int>(train[0].getCategory()), 1);

    ASSERT_EQ(train[1].getName(), "Container2");
    ASSERT_DOUBLE_EQ(train[1].getMass(), 30.0);
    ASSERT_DOUBLE_EQ(train[1].getVolume(), 5.0);
    ASSERT_EQ(static_cast<int>(train[1].getCategory()), 2);
}

TEST(TrainOutputOperator, OutputOperator) {
    Train train;
    train.setMaxWeight(100.0);
    train.setMaxVolume(200.0);

    Container container1("Container1", 10.0, 50.0, GAS);
    Container container2("Container2", 5.0, 30.0, BULK);

    train += container1;
    train += container2;

    std::ostringstream output;
    output << train;

    std::string expectedOutput = "Max Weight - 100; Max Volume - 200; Containers - 2\n";
    expectedOutput += "Container1 Mass - 50 Volume - 10 Category - GAS\n";
    expectedOutput += "Container2 Mass - 30 Volume - 5 Category - BULK\n\n";

    ASSERT_EQ(output.str(), expectedOutput);
}

TEST(TrainDeleteContainer, DeleteContainer) {
    Train train;
    train.setMaxWeight(300);
    train.setMaxVolume(200);
    Container container1("Container1", 10.0, 5.0, GAS);
    Container container2("Container2", 50.0, 3.0, BULK);
    Container container3("Container3", 8.0, 4.0, BULK);

    train += container1;
    train += container2;
    train += container3;

    train.deleteContainer(1);

    ASSERT_EQ(train.getCount(), 2);
    ASSERT_EQ(train[0].getName(), "Container1");
    ASSERT_EQ(train[1].getName(), "Container3");
}

TEST(TrainConstructor, CopyConstructor) {
    Train train1;
    Train train2 = train1;
    ASSERT_EQ(train2.getMaxWeight(), train1.getMaxWeight());
    ASSERT_EQ(train2.getMaxVolume(), train1.getMaxVolume());
    ASSERT_EQ(train2.getCount(), train1.getCount());
}


TEST(TrainMoveAssignmentOperator, MoveAssignmentOperator) {
    Train train1;
    train1.setMaxWeight(35.2);
    train1.setMaxVolume(21.1);
    train1.setCountContainers(3);

    Train train2;
    train2.setMaxWeight(50.0);
    train2.setMaxVolume(30.0);
    train2.setCountContainers(2);

    train2 = std::move(train1);

    ASSERT_DOUBLE_EQ(train2.getMaxWeight(), 35.2);
    ASSERT_DOUBLE_EQ(train2.getMaxVolume(), 21.1);
    ASSERT_EQ(train2.getCount(), 3);
}


TEST(TrainGettersAndSetters, GettersAndSetters) {
    Train train1;
    train1.setMaxWeight(325.2);
    train1.setMaxVolume(241.1);
    train1.setCountContainers(3);
    ASSERT_EQ(train1.getMaxWeight(), 325.2);
    ASSERT_EQ(train1.getMaxVolume(), 241.1);
    ASSERT_EQ(train1.getCount(), 3);
}

TEST(TrainCountMass, CountMass) {
    Container containers[] = {
            Container("Container1", 50.0, 10.0, PIECES),
            Container("Container2", 30.0, 5.0, BULK),
    };
    Train train(2, containers, 200.0, 100.0);
    ASSERT_EQ(train.countMass(), 15.0);
}

TEST(TrainCountCenterMass, CountCenterMass) {
    Container containers[] = {
            Container("Container1", 50.0, 10.0, PIECES),
            Container("Container2", 30.0, 10.0, BULK),
    };
    Train train(2, containers, 200.0, 100.0);
    ASSERT_EQ(train.countCenterMass(), 0.5);
}

TEST(TrainEnsuringSecurity, EnsuringSecurity) {
    Container containers[] = {
            Container("Container1", 50.0, 10.0, PIECES),
            Container("Container2", 30.0, 5.0, DANGEROUS),
            Container("Container3", 40.0, 6.0, BULK),
            Container("Container4", 25.0, 4.0, EMPTY),
            Container("Container5", 35.0, 7.0, DANGEROUS),
    };

    Train train(5, containers, 200.0, 100.0);
    train.ensuringSecurity();
    ASSERT_EQ(train.getContainers()[0].getCategory(), PIECES);
    ASSERT_EQ(train.getContainers()[1].getCategory(), DANGEROUS);

    ASSERT_EQ(train.getContainers()[3].getName(), "Container5");
    ASSERT_DOUBLE_EQ(train.getContainers()[3].getMass(), 7.0);
    ASSERT_DOUBLE_EQ(train.getContainers()[3].getVolume(), 35.0);
    ASSERT_EQ(train.getContainers()[3].getCategory(), DANGEROUS);
}

TEST(TrainTest, RemoveEmptyContainers) {
    Container containers[] = {
            Container(),
            Container("Container2", 30.0, 5.0, PIECES),
            Container(),
            Container("Container5", 35.0, 7.0, PIECES),
    };
    Train train(4, containers, 200.0, 100.0);

    train.minContainers();

    EXPECT_EQ(train.getCount(), 2);
    EXPECT_EQ(train.getContainers()[0].getCategory(), PIECES);
    EXPECT_EQ(train.getContainers()[1].getCategory(), PIECES);
}

TEST(TrainTest, AssignmentOperatorTest) {
    Container containers[] = {
            Container("Container1", 50.0, 10.0, PIECES),
            Container("Container2", 30.0, 5.0, DANGEROUS),
            Container("Container3", 40.0, 6.0, BULK),
            Container("Container4", 25.0, 4.0, EMPTY),
            Container("Container5", 35.0, 7.0, DANGEROUS),
    };
    Train train1(5, containers, 200.0, 100.0);
    Train train2;

    train2 = train1;

    EXPECT_EQ(train2.getMaxVolume(), train1.getMaxVolume());
    EXPECT_EQ(train2.getMaxWeight(), train1.getMaxWeight());
    EXPECT_EQ(train2.getCount(), train1.getCount());
}

TEST(TrainTest, AddingThrow) {
    Container containers[] = {
            Container("Container1", 50.0, 10.0, PIECES),
            Container("Container2", 30.0, 5.0, DANGEROUS),
            Container("Container3", 40.0, 6.0, BULK),
            Container("Container4", 25.0, 4.0, EMPTY),
            Container("Container5", 35.0, 7.0, DANGEROUS),
    };
    Train train1(5, containers, 200.0, 100.0);
    Container NewContainer("ContainerNew", 570.0, 110.0, PIECES);
    ASSERT_THROW(train1+=NewContainer, std::runtime_error);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
