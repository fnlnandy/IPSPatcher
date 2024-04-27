#include <iostream>
#include <string>
#include <vector>
#include "Global.hpp"
#include "BigEdian.hpp"
#include "Hunk.hpp"

#define ARRAY_COUNT(x) (sizeof(x) / sizeof(x)[0])

// Translates to "PATCH", this doesn't work if it is directly
// initalized as such though.
const u8 gMagicHeader[] = {0x50, 0x41, 0x54, 0x43, 0x48};
const size_t gMagicHeaderLength = ARRAY_COUNT(gMagicHeader);

static bool areBytesEqual(const u8 *bytes1, const u8 *bytes2, const size_t &length)
{
    for (size_t i = 0; i < length; i++)
    {
        if (bytes1[i] != bytes2[i])
            return false;
    }

    return true;
}

static std::vector<std::string> *parseArgs(int argc, char **argv)
{
    std::vector<std::string> *retVal = new std::vector<std::string>();

    if (argc == 0)
        return retVal;

    for (int i = 0; i < argc; i++)
        retVal->push_back(argv[i]);

    return retVal;
}

static std::string getArg(const std::vector<std::string> *args, const std::string &prefix)
{
    for (size_t i = 0, max = args->size(); i < max; i++)
    {
        const std::string &current = args->at(i);
        const size_t prefixLength = prefix.length();

        if (current.find(prefix) != 0)
            continue;
        if (current.length() > prefixLength)
            return current.substr(prefixLength);
        if (i + 1 < max)
            return args->at(i + 1);
    }

    return {""};
}

//! @todo Actually implement this
static int createIPSPatch(const std::vector<std::string> *args)
{
    const std::string sourceFileName = getArg(args, "-c");
    const std::string targetFileName = getArg(args, "-t");
    const std::string outputFileName = getArg(args, "-o");

    if (sourceFileName.empty())
        FATAL_ERROR("Empty -c argument provided.");
    if (targetFileName.empty())
        FATAL_ERROR("Empty -t argument provided.");
    if (outputFileName.empty())
        FATAL_ERROR("Empty -o argument provided.");

    BigEdian sourceFile = {sourceFileName, "rb"};
    BigEdian targetFile = {targetFileName, "rb"};
    BigEdian outputFile = {outputFileName, "wb"};

    // Mandatory header for the IPS File
    outputFile.writeBytes(gMagicHeader, gMagicHeaderLength);

    while (!sourceFile.isEnd() && !targetFile.isEnd())
    {
        Hunk diffHunk = Hunk::fromDiff(&sourceFile, &targetFile);

        // diffHunk.asIPS(&outputFile);
        std::cout << diffHunk << "\n";
    }

    return 0;
}

static int applyIPSPatch(const std::vector<std::string> *args)
{
    const std::string IPSFileName = getArg(args, "-p");
    const std::string fileToApplyOnFileName = getArg(args, "-a");

    if (IPSFileName.empty())
        FATAL_ERROR("Empty -p argument provided.");
    if (fileToApplyOnFileName.empty())
        FATAL_ERROR("Empty -a argument provided.");

    BigEdian IPSFile = {IPSFileName, "rb"};
    BigEdian fileToApplyOn = {fileToApplyOnFileName, "r+b"};

    if (!areBytesEqual(IPSFile.readBytes(gMagicHeaderLength), gMagicHeader, gMagicHeaderLength))
        FATAL_ERROR("The passed file is not a valid IPS Patch.");

    while (!IPSFile.isEnd())
    {
        Hunk toApply = Hunk::makeHunk(&IPSFile);
        toApply.write(&fileToApplyOn);

        std::cout << toApply << "\n";
    }

    return 0;
}

static int printUsage()
{
    std::printf("Usage: ipspatcher -m=[apply|a]|[create|c] [-p=PATCH] [-a=FILE] [-c=SOURCE] [-t=TARGET] [-o=OUTPUT PATCH]\n");
    return 0;
}

/**
 * -m: Mode = apply|a||create|c
 * -p: Pass an IPS patch = filename
 * -a: File to apply the IPS patch on = filename
 * -c: Source file = filename
 * -t: Target file = filename
 * -o: Output IPS patch = filename
 */
int main(int argc, char **argv)
{
    const std::vector<std::string> *args = parseArgs(--argc, ++argv);
    const std::string modeArg = getArg(args, "-m");

    if (modeArg == "apply" || modeArg == "a")
        return applyIPSPatch(args);
    if (modeArg == "create" || modeArg == "c")
        return createIPSPatch(args);

    return printUsage();
}