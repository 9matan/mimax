using System.IO;
using Sharpmake;

[Generate]
public class CommonProject : Project
{
    public CommonProject()
    {
        AddTargets(new Target(Platform.win64, DevEnv.vs2019, Optimization.Debug | Optimization.Release));
    }
	
	[Configure]
	public virtual void ConfigureAll(Project.Configuration conf, Target target)
	{
        conf.TargetPath = @"[project.SharpmakeCsPath]\..\..\Bin\[target.Platform]_[target.Optimization]";

        conf.Options.Add(Options.Vc.Compiler.Exceptions.Enable);
        conf.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);
    }
}

[Generate]
public class CommonSolution : Solution
{
    public CommonSolution()
    {
        AddTargets(new Target(Platform.win64, DevEnv.vs2019, Optimization.Debug | Optimization.Release));
    }

    [Configure]
    public virtual void ConfigureAll(Solution.Configuration conf, Target target)
    {
    }
}