
# Open Unreal Utilities

![OUU logo](./Resources/ouu.png)

The Open Unreal Utilities plugin is a general purpose utility plugin for Unreal Engine 4.

## Contents

The plugin is divided into several modules for different application purposes:

- **OUUBlueprintRuntime:** Blueprint exposed functionality that is already present in C++ UE4 off-the-shelf
- **OUUEditor:** Functionality that only works when running the Editor
- **OUURuntime:** Main runtime plugin. These utilties should be useful in all kinds of different contexts
	ranging from container templates, execution flow helpers, etc
- **OUUTests:** Automated tests that test test the plugin functionality. Only contains private content.
	You will never need to reference this from other modules!
- **OUUTestUtilities:** Testing utilities that help writing automated tests. Almost all of the tests in OUUTests
	use at least some of the functionality from here.
- **OUUUMG:** UMG Widgets and UI implementation helpers

For a more detailed overview of all utilities you should check out the plugin source code,
as most of the documentation is provided in the form of source code comments.
If you want to know how a type is meant to be used, it can be useful to check the automation tests.

## Workflows

### Versions

- Latest plugin version: 0.8.0
- Supported UE4 versions: 4.27

The plugin is still in a pre-1.0 development phase, so the API of many utilities is still up to change a lot.

#### Versions History (Summary)

Short summary of version changes so you don't have to sift through the entire history to find out what changed (roughly):

- **0.8.0**
	- Upgrade to UE 4.27
	- Added lexical operators for Blueprint
	- Simplified message log functions and added UE_LOG-like macros
	- Reworked bitmask utilty functions to work with both engine enums and custom enums that implement OUU bitmask traits
	- Debugging tools
		- Added gameplay debugger category type list which simplifies registering of debugger categories
		- Added enhanced ability system gameplay debugger category
		- Added actor map window
		- Moved all debugging tools into new module OUUDeveloper
- **0.7.0**
	- Added plugin logo (matching to Open Unreal Conventions)
	- Improved clang compatibility for PS4/Linux targets (still not guaranteed to run on those platforms)
- **0.6.0**
	- New Utilities: Spiral IDs, GarbageCollectionListener
	- Various smaller bug fixes, improvements and cleanups
- **0.5.1**
	- Stability Fixes
- **0.5.0**
	- New Utilities: Literal Gameplay Tags, WorldBoundSFSchedulerRegistry
	- Renamed/moved a bunch of utilities to reduce conflicts with other projects
- **0.4.0**
	- Various fixes and stability improvements
	- Unified plugin log category usages
- **0.3.0**
	- Upgrade to UE 4.26
	- New Utilities: Ring Aggregator, Sequential Frame Scheduler
	- Module Cleanup
- **0.2.0** Split repo into master and develop branches
- **0.1.0** Base set of utilities (All changes prior to 0.2.0 with no clear versioning strategy)

#### Increasing Version

Follow the following steps to promote changes from develop to master:

1. Increase version number to the next desired version in the following three files:
	- [README.md](./README.md) _(this file)_ - Plugin Version section
	- [OpenUnrealUtilities.uplugin](./OpenUnrealUtilities.uplugin) VersionName field
	- [.version](./.version)
2. Summarize the most important changes included with the new version in the history summary section above
3. Submit only these changes (no functional code changes included) with message "Increased version to major.minor.patch", e.g. "Increased version to 0.6.0"
4. Tag the submit with annotated tag that matches the version number: Tag and annotation message should match exactly, e.g. both could be "v0.6.0"
5. Fast-forward merge develop into master
6. Push develop, master _and(!)_ tags

### Branch Strategy

- Active development on develop
- (Semi) stable releases on master
- Tag releases with plugin version number

### Future Workflow Changes

- Add branches or tags for supported engine versions (e.g. UE4.25), which are updated along with releases
- No updates of deprecated code. As soon as the plugin breaks for an old engine version we drop support for it

## Automated Testing

A big majority of the utilities has unit tests that can be executed via the Session Frontend from within Unreal.
These tests are written with the help of testing utilities contained in the OpenUnrealUtilitiesTests module.
Said testing utilities are prepared for usage in other plugins or game code. The actual tests should serve as examples how
both the testing utilities as well as the targeted components are meant to be used.

### Getting Started (with Automated Testing)

If you want to use the testing utilities, it should be sufficient to

- include OUUTestUtilities.h and
- add the OUUTestUtilities module to the private dependencies of the module implementing the tests

### Overview

The automation testing utilities contain the following components:

- **FAutomationTestWorld** is a utility object that allows creation of a simple world and game framework objects for tests
- **AutomationTestParameterParser** is a utility object for complex automation tests that allows easy parsing of test parameters
which are supplied as a single FString per test case
- **CollectionTestFunctions** are test functions similar to the FAutomationTestBase member functions TestTrue(), TestEqual(), etc.
that allow comparing collections to expected result values while adding some additional checks and error messages to aid in debugging failed tests
- **OUUTestMacros** is a collection of macros that simplify the creation of tests
	- OUU_\* macros help declaring test cases with significantly less redundancy/boilerplate compared to test declaration macros provided out-of-the-box
	- SPEC_TEST_\* macros are wrappers for test functions to be used in [Automation Specs] without adding duplicate descriptions for every test (see below)
- **UOUUTestObject and UOUUTestWidget** are not-abstract classes derived directly from UObject and UUserWidget respectively available to use in tests

### Automation Specs

Unreal Engine v4.22 added a new test type called [Automation Spec](https://docs.unrealengine.com/en-US/Programming/Automation/AutomationSpec/index.html)
following the [Behavior Driven Design (BDD)](https://en.wikipedia.org/wiki/Behavior-driven_development) methodology.

We are convinced this new approach of writing tests is better than the previous one, so most of the plugin tests are written as specs adhering to
the engine source code convention of placing specs in \*.spec.cpp files. In cases where special test objects are required that need reflection code generated by
UHT, we put those into a FooTests.h header file next to spec file.

The macros used to define the specs are very lightweight and add minimal boilerplate, so we did not see any reason to add any macros for creating spec classes.
There is a set of macros for calling test functions though that wrap the automation test member functions as well the collection test functions added by the plugin.
All of these macros are prefixed with SPEC_TEST_ so they are easy to recognize and auto-complete. The macros do not introduce any hidden logic (early returns, etc),
but they auto-fill the test function description which is passed as "What" parameter. The aim when using these functions is to enforce that test descriptions should be
part of the expectations formulated using Describe and It, so the actual test calls should not need any additional descriptions.

Whenever you run into instances where the description of the test case is not sufficient to explain the call of test functions, it's a good sign that you should either rephrase
the description or break the test case down into separate smaller test cases that are easy to describe.

## Coding Conventions

The plugin adheres to the [Open Unreal Conventions](https://jonasreich.github.io/OpenUnrealConventions/) which are extended coding conventions based on Epic Games' coding guidelines. 

Some plugin specific conventions:
- Types or functions that are not namespaced which are likely to cause naming conflicts with future engine types or third/project code should be prefixed with OUU
- The full name of the plugin 'OpenUnrealUtilities' should be avoided to keep type names short and consistent
- All module names in the plugin must start with OUU prefix to make them easily distinguishable in Dependencies lists of Build.cs files

## Licensing

Both Open Unreal Utilities plugin and [sample project](https://github.com/JonasReich/OpenUnrealUtilitiesSampleProject) are licensed under the MIT license.

## Contributing

You are invited to create pull-requests to the github source for any additions or modifications you make to the plugin:
https://github.com/JonasReich/OpenUnrealUtilities
