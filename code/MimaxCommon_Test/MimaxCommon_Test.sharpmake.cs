using Sharpmake;

[Generate]
public class MimaxCommonTestProject : CommonProject
{
    public MimaxCommonTestProject()
    {
        Name = "MimaxCommon_Test";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.IncludePaths.Add(@"[project.SourceRootPath]");

        conf.AddPrivateDependency<MimaxCommonProject>(target);
		
        conf.Output = Configuration.OutputType.Lib;
    }
}