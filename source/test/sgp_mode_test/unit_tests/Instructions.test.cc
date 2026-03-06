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
  GIVEN("A program that only does increment"){
  //Creates a program that only does Increment
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
  WHEN("An organism runs increment on 0"){
    //Runs the Functor, thus runs the increment instruction on the host
    Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
    THEN("The first item in the register is 1"){
      REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] == 1);
    }
  }
  }
}


TEST_CASE("Test Decrement", "[idk]") {
  GIVEN("A program that only does Decrement"){
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
  WHEN("An argument runs decrement on 99"){
    //Runs the Functor, thus runs the Decrement instruction on the host
    Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
    THEN("The first item in the register is 99"){
      REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] == 98);
    }
  }
  }
}


TEST_CASE("Test Add", "[idk]") {
  GIVEN("A program that only does add"){
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

  WHEN("An organism runs subtract with inputs of 1 and 2"){
    host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[1]] = 1.0f;
    host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[2]] = 2.0f;
    
    //Runs the Functor, thus runs the Add instruction on the host
    Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
    THEN("The first item in the register is 3"){
      REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] == 3);
    }
  }
  }
}


TEST_CASE("Test Subtract", "[idk]") {
  GIVEN("A program that only does subtract"){
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
  WHEN("An organism runs subtract with inputs of 3 and 2"){
  //Runs the Functor, thus runs the Subtract instruction on the host
  Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
  THEN("The first item in the register is 1"){
    REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] == 1);
  }
  }
  }
}

TEST_CASE("Test Nand", "[idk]") {
  GIVEN("A program that only does Nand"){
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

    
    WHEN("An organism runs the nand instruction on two of the same number"){
      host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] = std::bit_cast<float>(0b0101011110011010000010111111011);
      host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[1]] = std::bit_cast<float>(0b0101011110011010000010111111011);
      
      //Runs the Functor, thus runs the Nand instruction on the host
      Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
      THEN("The first value of the register is set to all 0s"){
        REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[2]] == 0);
      }
    }
  }
}

TEST_CASE("Test Steal", "[idk]") {

  GIVEN("A program that only does steal"){
    //Creates a program that only does Steal
    const sgpl::Program<Spec> program(R"(
      {
        "value0": [
          {
            "operation": "Steal",
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
    config.DONATION_STEAL_INST(1);
    config.INTERACTION_MECHANISM(1);
    config.SYMBIONT_TYPE(1);

    SGPWorld world(random, &config, LogicTasksDiff);
    emp::Ptr<HealthHost> host = emp::NewPtr<HealthHost>(&random, &world, &config, program);
    emp::Ptr<SGPSymbiont> parasite_symbiont = emp::NewPtr<SGPSymbiont> (&random, &world, &config, program);
    host->AddSymbiont(parasite_symbiont);
    world.AddOrgAt(host, 0);
    //1 is the op_code of Steal
    const int steal_code = 12;

    //Creates a Functor of the run function of the instruction using the op_code of Nand
    //PROBLEM AREA - PROB
    using library_t = typename Spec::library_t;
    using Functor = typename library_t::template Operation<steal_code>;

    WHEN("A health symbiont runs the steal instruction"){
      //Runs the Functor, thus runs the Nand instruction on the host
      Functor::template run<Spec>(parasite_symbiont->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, parasite_symbiont->GetCPU().state);
      THEN("The health host is set to give a cycle to its symbiont"){
        REQUIRE(host->GetCyclesGiven() < 0);
      }
    }

  }
}

TEST_CASE("Test Donate", "[idk]") {

  GIVEN("A program that only does donate"){
    //Creates a program that only does Donate
    const sgpl::Program<Spec> program(R"(
      {
        "value0": [
          {
            "operation": "Donate",
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
    config.DONATION_STEAL_INST(1);
    config.INTERACTION_MECHANISM(1);
    config.SYMBIONT_TYPE(0);

    SGPWorld world(random, &config, LogicTasksDiff);
    emp::Ptr<HealthHost> host = emp::NewPtr<HealthHost>(&random, &world, &config, program);
    emp::Ptr<SGPSymbiont> mutualist_symbiont = emp::NewPtr<SGPSymbiont> (&random, &world, &config, program);
    host->AddSymbiont(mutualist_symbiont);
    world.AddOrgAt(host, 0);
    //1 is the op_code of Donate
    const int donate_code = 11;

    //Creates a Functor of the run function of the instruction using the op_code of Donate
    //PROBLEM AREA - PROB
    using library_t = typename Spec::library_t;
    using Functor = typename library_t::template Operation<donate_code>;

    
    //Runs the Functor, thus runs the Nand instruction on the host
    WHEN("A health symbiont runs the donate instruction"){
      Functor::template run<Spec>(mutualist_symbiont->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, mutualist_symbiont->GetCPU().state);
      THEN("The host is set to receive a cycle from its symbiont"){
        REQUIRE(host->GetCyclesGiven() > 0);
      }
    }
  }

}

TEST_CASE("Test IOSense", "[inst-test]") {

  //Creates a program that only does IOSense
  const sgpl::Program<Spec> program(R"(
    {
      "value0": [
        {
          "operation": "IOSense",
          "args": {
            "value0": 0,
            "value1": 1,
            "value2": 2
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


   //1 is the op_code of IOSense
  const int iosense_code = 14;

  //Creates a Functor of the run function of the instruction using the op_code of IOSense
  //PROBLEM AREA - PROB
  using library_t = typename Spec::library_t;
  using Functor = typename library_t::template Operation<iosense_code>;

  host->GetCPU().state.input_buf.push(734856699);
  host->GetCPU().state.input_buf.push(1177728054);

  WHEN("Host completes a not poisoned task"){
    long not_output = 3560110596;
    //Host completes a 
    host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] = not_output;

    THEN("IOSense returns a 1"){
      //Runs the Functor, thus runs the IOSense instruction on the host
      Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
      REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[1] == 1);
    }
  }
  WHEN("Host completes a poisoned task"){
    long nor_output = 2415938048;
    //Host completes a 
    host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[0]] = nor_output;

    THEN("IOSense returns a -1"){
      //Runs the Functor, thus runs the IOSense instruction on the host
      Functor::template run<Spec>(host->GetCPU().GetCPUPointer().GetActiveCore(), program[0], program, host->GetCPU().state);
      REQUIRE(host->GetCPU().GetCPUPointer().GetActiveCore().registers[program[0].args[1]] == -1);
    }
  }

}


