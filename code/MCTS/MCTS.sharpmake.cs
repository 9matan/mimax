using Sharpmake;

[Generate]
public class MCTSProject : CommonProject
{
    public MCTSProject()
    {
        Name = "MCTS";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.PrecompHeader = "MCTS_PCH.h";
        conf.PrecompSource = "MCTS_PCH.cpp";

        conf.IncludePaths.Add(@"[project.SourceRootPath]");

        conf.Output = Configuration.OutputType.Lib;
    }
}