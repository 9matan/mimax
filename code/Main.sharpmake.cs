using Sharpmake;

[module: Sharpmake.Include(@"Mimax_TestAll\Mimax_TestAll.sharpmake.cs")]
[module: Sharpmake.Include(@"MimaxCommon\MimaxCommon.sharpmake.cs")]
[module: Sharpmake.Include(@"MimaxCommon_Test\MimaxCommon_Test.sharpmake.cs")]
[module: Sharpmake.Include(@"Minimax\Minimax.sharpmake.cs")]
[module: Sharpmake.Include(@"Minimax_Test\Minimax_Test.sharpmake.cs")]
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
		
		conf.AddProject<MimaxCommonProject>(target);
		conf.AddProject<MinimaxProject>(target);

		conf.AddProject<MimaxCommonTestProject>(target);
		conf.AddProject<MimaxTestAllProject>(target);
		conf.AddProject<MinimaxTestProject>(target);
		
		conf.SetStartupProject<MimaxTestAllProject>();
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