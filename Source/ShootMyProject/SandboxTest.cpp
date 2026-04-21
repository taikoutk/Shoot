// Fill out your copyright notice in the Description page of Project Settings.

#include "SandboxTest.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "STest.Math.MaxInt",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "STest.Math.Sqrt",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOneNotTwo, "STest.Math.OneNotTwo",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FMathMaxInt::RunTest(const FString& Parameters)
{
	AddInfo("Max [int] func testing");
	TestTrue("2 different posittive numbers", FMath::Max(13, 25) == 25);
	TestTrue("2 different negative numbers", FMath::Max(-13, -25) == -13);
	return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
	AddInfo("Sqrt func testing");
	TestEqual("Sqrt(4) [0] ", FMath::Sqrt(4.0f), 2.0f);
	TestEqual("Sqrt(3) [0] ", FMath::Sqrt(3.0f), 1.73205f, 1.e-5f);
	return true;
}

bool FOneNotTwo::RunTest(const FString& Parameters)
{
	if (1 != 2)
	{
		AddWarning("1 not equal 2");
	}
	return true;
}