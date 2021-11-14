using System.Text.RegularExpressions;

using Sharpmake;

[Generate]
public class GTestProject : CommonProject
{
    public GTestProject()
    {
        Name = "GTest";
        SourceRootPath = @"[project.SharpmakeCsPath]";

        SourceFilesExcludeRegex.Add(
            Regex.Escape(
                ResolveString(@"[project.SourceRootPath]\src\gtest\src")));
        SourceFilesExcludeRegex.Add(
            Regex.Escape(
                ResolveString(@"[project.SourceRootPath]\src\gmock\src")));

        SourceFiles.Add(@"[project.SourceRootPath]\src\gtest\gtest-all.cc");
        SourceFiles.Add(@"[project.SourceRootPath]\src\gmock\gmock-all.cc");
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.IncludePaths.Add(@"[project.SourceRootPath]\include");

        conf.Output = Configuration.OutputType.Lib;
    }
}