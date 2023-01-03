#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QDebug>

#include <QtCore/QDateTime>


/// 声明一个结构体用于解析参数信息
struct ParseInfo {
    int option_run_mode = 5;
    bool option_time = false;
    bool option_date = false;
    QString position_arg_urls;
    QString position_arg_user_name;
};

enum CommandLineParseResult
{
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
    CommandLineHelpRequested
};


CommandLineParseResult parseCommandLine(QCommandLineParser &parser, ParseInfo *info, QString *errorMessage)
{
    if(!info) {
        qDebug() << "info is null";
        return CommandLineError;
    }

    /// 替换当前默认的模式
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.setApplicationDescription("这是一个用于展示QCommandLineParser功能的例子");

    /// @brief 添加一个选项 用于显示时间 即 -t 或者 --time
    QCommandLineOption showTimeOption(QStringList() << "t" << "show-time", "显示当前时间");
    parser.addOption(showTimeOption);

    /// @brief 添加一个短选项 用于显示日期
    QCommandLineOption showDateOption("d", "显示当前日期");
    parser.addOption(showDateOption);

    /// @brief 添加一个长选项 用于设置运行模式 模式值为 1-5，默认为5 该选项为必须项
    QCommandLineOption runModeOption( "run-mode", "设置运行模式", "5");
    parser.addOption(runModeOption);


    /// @brief 添加一个带有附加参数（如定义一个长用于描述我是谁），可显示在帮助信息中
    /// 展示为
    // Usage: xxxx
    //
    // Arguments:
    //   user       user name
    parser.addPositionalArgument("user", "user name","[user name]");


    /// @brief 添加一个带有附加参数如定义一个长选项urls用于描述链接），并附加一个名称显示在Usage 行
    /// 展示为
    // Usage: xxxx [urls...]
    //
    // Arguments:
    //   urls       定义一个链接
    parser.addPositionalArgument("urls", "URLs to open, optionally.", "[urls...]");


    /// @brief 添加一个help选项
    const QCommandLineOption helpOption = parser.addHelpOption();
    const QCommandLineOption versionOption = parser.addVersionOption();

    if (!parser.parse(QCoreApplication::arguments())) {
        *errorMessage = parser.errorText();
        return CommandLineError;
    }

    if (parser.isSet(versionOption))
        return CommandLineVersionRequested;

    if (parser.isSet(helpOption))
        return CommandLineHelpRequested;

    if(parser.isSet(runModeOption)){
        const int runMode = parser.value(runModeOption).toInt();
        if(runMode < 1 || runMode > 5){
            *errorMessage = "运行模式只能为1-5";
            return CommandLineError;
        }
        info->option_run_mode = runMode;
    }else{
        qDebug()<<"Warning: 未设置运行模式，将使用默认值 5 ";
    }

    info->option_time = parser.isSet(showTimeOption);
    info->option_date = parser.isSet(showDateOption);

    const QStringList positionalArguments = parser.positionalArguments();
    if (positionalArguments.isEmpty()) {
        *errorMessage = "Argument 'user' missing.";
        return CommandLineError;
    }
    int nSizePositionalArguments = positionalArguments.size();
    if(nSizePositionalArguments == 2){
        info->position_arg_user_name = positionalArguments.at(0);
        info->position_arg_urls = positionalArguments.at(1);
    }else{
        *errorMessage = "Argument 'user , urls' missing.";
        return CommandLineError;
    }


    return CommandLineOk;
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("QCommandLineParser-Program");
    QCoreApplication::setApplicationVersion("1.0");
    QCommandLineParser parser;
    ParseInfo info;
    QString errorMessage;
    const CommandLineParseResult parseResult = parseCommandLine(parser, &info, &errorMessage);
    switch (parseResult)
    {
    case CommandLineOk:
        break;
    case CommandLineError:
        fputs(qPrintable(errorMessage), stderr);
        fputs("\n\n", stderr);
        fputs(qPrintable(parser.helpText()), stderr);
        return 1;
    case CommandLineVersionRequested:
        printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
               qPrintable(QCoreApplication::applicationVersion()));
        return 0;
    case CommandLineHelpRequested:
        parser.showHelp();
        Q_UNREACHABLE();
    }

    /// Do Parse info
    /// show date
    if(info.option_date){
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd");
    }
    /// show time
    if(info.option_time){
        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss");
    }
    /// show run mode
    qDebug() << "run mode: " << info.option_run_mode;
    /// show user name
    qDebug() << "user name: " << info.position_arg_user_name;
    /// show urls
    qDebug() << "urls: " << info.position_arg_urls;
    return 0;
}