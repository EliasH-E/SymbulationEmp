#include "../../../sgp_mode/GenomeLibrary.h"
#include "../../../sgp_mode/CPU.h"
#include "../../../sgp_mode/HealthHost.h"
#include "../../../sgp_mode/SGPWorld.h"
#include "../../../sgp_mode/SGPWorldSetup.cc"
#include "../../../sgp_mode/SGPConfigSetup.h"
#include "../../../sgp_mode/SGPHost.cc"
#include "../../../sgp_mode/SGPHost.h"
#include "../../../sgp_mode/Instructions.h"
#include "../../../sgp_mode/SGPSymbiont.h"
#include "../../../default_mode/WorldSetup.cc"
#include "../../../default_mode/DataNodes.h"
#include "../../../sgp_mode/SGPDataNodes.h"

#include "../../../sgp_mode/CPUState.h"


#include "../../../../signalgp-lite/include/sgpl/algorithm/execute_core.hpp"
#include "../../../../signalgp-lite/include/sgpl/hardware/Core.hpp"
#include "../../../../signalgp-lite/include/sgpl/library/OpLibrary.hpp"
#include "../../../../signalgp-lite/include/sgpl/operations/unary/Increment.hpp"
#include "../../../../signalgp-lite/include/sgpl/program/Program.hpp"
#include "../../../../signalgp-lite/include/sgpl/spec/Spec.hpp"


TEST_CASE("Test Increment", "[idk]") {

  //Creates a program that only does Decrement
  const sgpl::Program<Spec> program(R"(
    {
      "value0": [
        {
          "operation": "Increment",
          "args": {
            "value0": 0,
            "value1": 0,
            "value2": 0
          },
          "bitstring": "0000000000000000000000000000000000000000000000000000000000000000",
          "descriptors": []
        }
      ]
    }
  )");

  emp::Random random(1);
  SymConfigSGP config;
  config.SEED(1);
  config.MUTATION_RATE(0.0);
  config.MUTATION_SIZE(0.002);
  config.TRACK_PARENT_TASKS(1);
  config.VT_TASK_MATCH(1);
  config.HOST_ONLY_FIRST_TASK_CREDIT(0);
  config.SYM_ONLY_FIRST_TASK_CREDIT(0);
  config.DIFFERENT_TASK_VALUES(1);

  SGPWorld world(random, &config, LogicTasksDiff);

  emp::Ptr<SGPHost> host = emp::NewPtr<SGPHost>(&random, &world, &config, program);

  //3 is the op_code of Increment
  const int increment_code = 2;

  //Creates a Functor of the run function of the instruction using the op_code of increment
  using library_t = typename Spec::library_t;
  using Functor = typename library_t::template Operation<increment_code>;
  
  //Runs the Functor, thus runs the increment instruction on the host
  Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
  REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] == 1);

}


TEST_CASE("Test Decrement", "[idk]") {

  //Creates a program that only does Decrement
  const sgpl::Program<Spec> program(R"(
    {
      "value0": [
        {
          "operation": "Decrement",
          "args": {
            "value0": 0,
            "value1": 0,
            "value2": 0
          },
          "bitstring": "0000000000000000000000000000000000000000000000000000000000000000",
          "descriptors": []
        }
      ]
    }
  )");

  emp::Random random(1);
  SymConfigSGP config;
  config.SEED(1);
  config.MUTATION_RATE(0.0);
  config.MUTATION_SIZE(0.002);
  config.TRACK_PARENT_TASKS(1);
  config.VT_TASK_MATCH(1);
  config.HOST_ONLY_FIRST_TASK_CREDIT(0);
  config.SYM_ONLY_FIRST_TASK_CREDIT(0);
  config.DIFFERENT_TASK_VALUES(1);

  SGPWorld world(random, &config, LogicTasksDiff);

  emp::Ptr<SGPHost> host = emp::NewPtr<SGPHost>(&random, &world, &config, program);


   //4 is the op_code of Decrement
  const int decrement_code = 3;

  //Creates a Functor of the run function of the instruction using the op_code of Decrement
  using library_t = typename Spec::library_t;
  using Functor = typename library_t::template Operation<decrement_code>;

  host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] = 99;
  
  //Runs the Functor, thus runs the Decrement instruction on the host
  Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
  REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] == 98);

}

TEST_CASE("Test BitwiseShift", "[idk]") {

  //Creates a program that only does BitwiseShift
  //A reminder, in this naming convention uppercase letter denotes a break in words
  //Thus BitwiseShift has the name Bitwise Shift, if BitwiseShift is used, the test will crash
  const sgpl::Program<Spec> program(R"(
    {
      "value0": [
        {
          "operation": "Bitwise Shift",
          "args": {
            "value0": 2,
            "value1": 0,
            "value2": 1
          },
          "bitstring": "0000000000000000000000000000000000000000000000000000000000000000",
          "descriptors": []
        }
      ]
    }
  )");

  emp::Random random(1);
  SymConfigSGP config;
  config.SEED(1);
  config.MUTATION_RATE(0.0);
  config.MUTATION_SIZE(0.002);
  config.TRACK_PARENT_TASKS(1);
  config.VT_TASK_MATCH(1);
  config.HOST_ONLY_FIRST_TASK_CREDIT(0);
  config.SYM_ONLY_FIRST_TASK_CREDIT(0);
  config.DIFFERENT_TASK_VALUES(1);

  SGPWorld world(random, &config, LogicTasksDiff);

  emp::Ptr<SGPHost> host = emp::NewPtr<SGPHost>(&random, &world, &config, program);


   //1 is the op_code of BitwiseShift
  const int bitwiseshift_code = 1;

  //Creates a Functor of the run function of the instruction using the op_code of BitwiseShift
  //PROBLEM AREA - PROB
  using library_t = typename Spec::library_t;
  using Functor = typename library_t::template Operation<bitwiseshift_code>;

  
  host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[1]] = std::bit_cast<float>(0b010001);
  host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[2]] = -1*std::bit_cast<float>(0b000001);
  
  //Runs the Functor, thus runs the BitwiseShift instruction on the host
  Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
  REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] == 3);

}

TEST_CASE("Test Add", "[idk]") {

  //Creates a program that only does Add
  const sgpl::Program<Spec> program(R"(
    {
      "value0": [
        {
          "operation": "Add",
          "args": {
            "value0": 2,
            "value1": 0,
            "value2": 1
          },
          "bitstring": "0000000000000000000000000000000000000000000000000000000000000000",
          "descriptors": []
        }
      ]
    }
  )");

  emp::Random random(1);
  SymConfigSGP config;
  config.SEED(1);
  config.MUTATION_RATE(0.0);
  config.MUTATION_SIZE(0.002);
  config.TRACK_PARENT_TASKS(1);
  config.VT_TASK_MATCH(1);
  config.HOST_ONLY_FIRST_TASK_CREDIT(0);
  config.SYM_ONLY_FIRST_TASK_CREDIT(0);
  config.DIFFERENT_TASK_VALUES(1);

  SGPWorld world(random, &config, LogicTasksDiff);

  emp::Ptr<SGPHost> host = emp::NewPtr<SGPHost>(&random, &world, &config, program);


   //1 is the op_code of Add
  const int add_code = 4;

  //Creates a Functor of the run function of the instruction using the op_code of Add
  //PROBLEM AREA - PROB
  using library_t = typename Spec::library_t;
  using Functor = typename library_t::template Operation<add_code>;

  
  host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[1]] = 1.0f;
  host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[2]] = 2.0f;
  
  //Runs the Functor, thus runs the Add instruction on the host
  Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
  REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] == 3);

}


TEST_CASE("Test Subtract", "[idk]") {

  //Creates a program that only does Add
  const sgpl::Program<Spec> program(R"(
    {
      "value0": [
        {
          "operation": "Subtract",
          "args": {
            "value0": 2,
            "value1": 0,
            "value2": 1
          },
          "bitstring": "0000000000000000000000000000000000000000000000000000000000000000",
          "descriptors": []
        }
      ]
    }
  )");

  emp::Random random(1);
  SymConfigSGP config;
  config.SEED(1);
  config.MUTATION_RATE(0.0);
  config.MUTATION_SIZE(0.002);
  config.TRACK_PARENT_TASKS(1);
  config.VT_TASK_MATCH(1);
  config.HOST_ONLY_FIRST_TASK_CREDIT(0);
  config.SYM_ONLY_FIRST_TASK_CREDIT(0);
  config.DIFFERENT_TASK_VALUES(1);

  SGPWorld world(random, &config, LogicTasksDiff);

  emp::Ptr<SGPHost> host = emp::NewPtr<SGPHost>(&random, &world, &config, program);


   //1 is the op_code of Subtract
  const int sub_code = 5;

  //Creates a Functor of the run function of the instruction using the op_code of Subtract
  //PROBLEM AREA - PROB
  using library_t = typename Spec::library_t;
  using Functor = typename library_t::template Operation<sub_code>;

  
  host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[1]] = 3.0f;
  host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[2]] = 2.0f;
  
  //Runs the Functor, thus runs the Subtract instruction on the host
  Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
  REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] == 1);

}

TEST_CASE("Test Nand", "[idk]") {

  //Creates a program that only does Nand
  const sgpl::Program<Spec> program(R"(
    {
      "value0": [
        {
          "operation": "Nand",
          "args": {
            "value0": 2,
            "value1": 1,
            "value2": 0
          },
          "bitstring": "0000000000000000000000000000000000000000000000000000000000000000",
          "descriptors": []
        }
      ]
    }
  )");

  emp::Random random(1);
  SymConfigSGP config;
  config.SEED(1);
  config.MUTATION_RATE(0.0);
  config.MUTATION_SIZE(0.002);
  config.TRACK_PARENT_TASKS(1);
  config.VT_TASK_MATCH(1);
  config.HOST_ONLY_FIRST_TASK_CREDIT(0);
  config.SYM_ONLY_FIRST_TASK_CREDIT(0);
  config.DIFFERENT_TASK_VALUES(1);

  SGPWorld world(random, &config, LogicTasksDiff);

  emp::Ptr<SGPHost> host = emp::NewPtr<SGPHost>(&random, &world, &config, program);


   //1 is the op_code of Nand
  const int nand_code = 10;

  //Creates a Functor of the run function of the instruction using the op_code of Nand
  //PROBLEM AREA - PROB
  using library_t = typename Spec::library_t;
  using Functor = typename library_t::template Operation<nand_code>;

  
  host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] = std::bit_cast<float>(0b0101011110011010000010111111011);
  host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[1]] = std::bit_cast<float>(0b0101011110011010000010111111011);
  
  //Runs the Functor, thus runs the Nand instruction on the host
  Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
  REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[2]] == 0);

}


