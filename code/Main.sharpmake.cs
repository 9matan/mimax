using Sharpmake;

[module: Sharpmake.Include(@"..\external\gtest\GTest.sharpmake.cs")]

[module: Sharpmake.Include(@"Mimax\Mimax.sharpmake.cs")]
[module: Sharpmake.Include(@"Mimax_Test\Mimax_Test.sharpmake.cs")]
[module: Sharpmake.Include(@"Sharpmake\Common.sharpmake.cs")]

[Generate]
public class MimaxSolution : CommonSolution
{
	public MimaxSolution()
	{
		Name = "MimaxSolution";
	}

	public override void ConfigureAll(Solution.Configuration conf, Target target)
    {
		base.ConfigureAll(conf, target);
		
        conf.SolutionPath = @"[solution.SharpmakeCsPath]\Sharpmake\generated";

		conf.AddProject<MimaxProject>(target);
		conf.AddProject<MimaxTestProject>(target);
		
		conf.SetStartupProject<MimaxTestProject>();
    }
}

public static class Main
{
    [Sharpmake.Main]
    public static void SharpmakeMain(Sharpmake.Arguments arguments)
    {
		KitsRootPaths.SetUseKitsRootForDevEnv(DevEnv.vs2019, KitsRootEnum.KitsRoot10, Options.Vc.General.WindowsTargetPlatformVersion.Latest);

		arguments.Generate<MimaxSolution>();
    }
}