using Sharpmake;

[Generate]
public class MCTSTestProject : CommonProject
{
    public MCTSTestProject()
    {
        Name = "MCTS_Test";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.IncludePaths.Add(@"[project.SourceRootPath]");

        conf.AddPrivateDependency<MimaxCommonTestProject>(target);
        conf.AddPrivateDependency<MCTSProject>(target);
		
        conf.Output = Configuration.OutputType.Lib;
    }
}