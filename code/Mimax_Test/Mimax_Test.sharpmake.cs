using Sharpmake;

[Generate]
public class MimaxTestProject : CommonProject
{
    public MimaxTestProject()
    {
        Name = "Mimax_Test";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

		conf.IncludePaths.Add(@"[project.SourceRootPath]");

        conf.AddPrivateDependency<MimaxProject>(target);
    }
}