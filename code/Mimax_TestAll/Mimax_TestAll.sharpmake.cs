using Sharpmake;

[Generate]
public class MimaxTestAllProject : CommonProject
{
    public MimaxTestAllProject()
    {
        Name = "Mimax_TestAll";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.AddPrivateDependency<MimaxCommonTestProject>(target);
        conf.AddPrivateDependency<MinimaxTestProject>(target);
    }
}