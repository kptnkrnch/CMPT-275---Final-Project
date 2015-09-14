USE [c275g09A]
GO
/****** Object:  Table [dbo].[Activity]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Activity](
	[activityID] [int] IDENTITY(1,1) NOT NULL,
	[courseID] [int] NULL,
	[activityName] [varchar](100) NULL,
	[pathToGradeFile] [varchar](500) NULL,
	[pathToWorkToMark] [varchar](500) NULL,
	[pathToSolutionFile] [varchar](500) NULL,
	[language] [varchar](50) NULL,
	[dueDate] [date] NULL,
	[isProgrammingActivity] [bit] NULL,
	[isEssayActivity] [bit] NULL,
	[isProblemSetActivity] [bit] NULL,
	[isMultipleChoiceActivity] [bit] NULL,
PRIMARY KEY CLUSTERED 
(
	[activityID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ActivityFile]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ActivityFile](
	[fileNumber] [int] IDENTITY(1,1) NOT NULL,
	[activityID] [int] NOT NULL,
	[fileName] [varchar](200) NOT NULL,
	[fileType] [varchar](10) NOT NULL,
 CONSTRAINT [PK_ActivityFile] PRIMARY KEY CLUSTERED 
(
	[fileNumber] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Course]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Course](
	[courseID] [int] IDENTITY(1,1) NOT NULL,
	[courseNumber] [varchar](10) NULL,
	[courseName] [varchar](100) NULL,
	[startDate] [date] NULL,
	[endDate] [date] NULL,
	[instructorID] [varchar](10) NULL,
PRIMARY KEY CLUSTERED 
(
	[courseID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Marker]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Marker](
	[userID] [varchar](10) NOT NULL,
	[courseID] [int] NOT NULL,
	[privilegeLevel] [int] NULL,
PRIMARY KEY CLUSTERED 
(
	[userID] ASC,
	[courseID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MultipleChoiceAnswer]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[MultipleChoiceAnswer](
	[answerID] [int] IDENTITY(1,1) NOT NULL,
	[multipleChoiceAnswerKeyID] [int] NOT NULL,
	[value] [char](1) NULL,
	[weight] [float] NULL,
 CONSTRAINT [PK_MultipleChoiceAnswer] PRIMARY KEY CLUSTERED 
(
	[answerID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MultipleChoiceAnswerKey]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[MultipleChoiceAnswerKey](
	[multipleChoiceAnswerKeyID] [int] IDENTITY(1,1) NOT NULL,
	[activityID] [int] NULL,
PRIMARY KEY CLUSTERED 
(
	[multipleChoiceAnswerKeyID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[ProgrammingActivity]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ProgrammingActivity](
	[activityID] [int] NOT NULL,
	[programmingLanguage] [varchar](50) NULL,
	[compilerEnvironment] [varchar](100) NULL,
PRIMARY KEY CLUSTERED 
(
	[activityID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ProgrammingTest]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ProgrammingTest](
	[programmingTestID] [int] IDENTITY(1,1) NOT NULL,
	[activityID] [int] NOT NULL,
	[name] [varchar](100) NOT NULL,
	[pathToConsoleInput] [varchar](500) NOT NULL,
	[pathToConsoleOutput] [varchar](500) NOT NULL,
	[pathToConsoleError] [varchar](500) NULL,
	[compileCommand] [varchar](500) NULL,
	[environment] [varchar](500) NULL,
	[timeout] [int] NOT NULL,
 CONSTRAINT [PK_ProgrammingTest] PRIMARY KEY CLUSTERED 
(
	[programmingTestID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ProgrammingTestInputs]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ProgrammingTestInputs](
	[testFileID] [int] IDENTITY(1,1) NOT NULL,
	[programmingTestID] [int] NOT NULL,
	[filePath] [varchar](500) NOT NULL,
 CONSTRAINT [PK_ProgrammingTestInputs] PRIMARY KEY CLUSTERED 
(
	[testFileID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ProgrammingTestOutputs]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ProgrammingTestOutputs](
	[testFileID] [int] IDENTITY(1,1) NOT NULL,
	[programmingTestID] [int] NOT NULL,
	[filePath] [varchar](500) NOT NULL,
 CONSTRAINT [PK_ProgrammingTestOutputs] PRIMARY KEY CLUSTERED 
(
	[testFileID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Rubric]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Rubric](
	[rubricID] [int] IDENTITY(1,1) NOT NULL,
	[activityID] [int] NULL,
PRIMARY KEY CLUSTERED 
(
	[rubricID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[RubricItem]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[RubricItem](
	[rubricItemID] [int] IDENTITY(1,1) NOT NULL,
	[rubricID] [int] NULL,
	[rubricItemText] [nchar](100) NULL,
	[rubricItemValue] [float] NULL,
 CONSTRAINT [PK_RubricItem] PRIMARY KEY CLUSTERED 
(
	[rubricItemID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[RubricItemGrade]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[RubricItemGrade](
	[gradeID] [int] IDENTITY(1,1) NOT NULL,
	[studentID] [varchar](10) NOT NULL,
	[rubricItemID] [int] NOT NULL,
	[mark] [float] NULL,
	[groupID] [varchar](10) NULL,
	[markerPrivilege] [int] NULL,
	[markerID] [varchar](10) NULL,
 CONSTRAINT [PK_RubricItemGrade] PRIMARY KEY CLUSTERED 
(
	[gradeID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[RubricSection]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[RubricSection](
	[rubricID] [int] NOT NULL,
	[sectionID] [int] NOT NULL,
	[expectedOutcome] [varchar](max) NULL,
	[weight] [float] NULL,
PRIMARY KEY CLUSTERED 
(
	[rubricID] ASC,
	[sectionID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Student]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Student](
	[studentID] [varchar](10) NOT NULL,
	[courseID] [int] NOT NULL,
	[firstName] [varchar](50) NOT NULL,
	[middleName] [varchar](50) NULL,
	[lastName] [varchar](100) NOT NULL,
 CONSTRAINT [PK__Student__4FBB52A3DB155540] PRIMARY KEY CLUSTERED 
(
	[studentID] ASC,
	[courseID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Users]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Users](
	[userID] [varchar](10) NOT NULL,
	[employeeID] [varchar](10) NULL,
	[password] [varchar](50) NULL,
	[email] [varchar](100) NULL,
	[firstName] [varchar](50) NULL,
	[middleName] [varchar](50) NULL,
	[lastName] [varchar](50) NULL,
	[isInstructor] [bit] NULL,
	[isSystemAdministrator] [bit] NULL,
	[isAdministrativeAssistant] [bit] NULL,
	[isTeachingAssistant] [bit] NULL,
	[isAdministrator] [bit] NULL,
	[isBlocked] [bit] NULL,
	[resetPassword] [bit] NULL,
	[loginAttempts] [int] NULL CONSTRAINT [DF_Users_loginAttempts]  DEFAULT ((0)),
PRIMARY KEY CLUSTERED 
(
	[userID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[UserSession]    Script Date: 8/5/2015 10:10:27 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[UserSession](
	[sessionID] [int] IDENTITY(1,1) NOT NULL,
	[userID] [varchar](10) NULL,
	[sessionCode] [int] NULL,
	[role] [int] NULL,
	[lastActive] [datetime] NULL,
 CONSTRAINT [PK_UserSession] PRIMARY KEY CLUSTERED 
(
	[sessionID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO

INSERT INTO Users (userID, password, isInstructor, isAdministrativeAssistant, isSystemAdministrator, isTeachingAssistant, isAdministrator, isBlocked, resetPassword)
VALUES ('11111','11111', '0', '0', '1', '0', '0', '0', '1');