import * as tmp from "tmp";
import * as fs from "fs";
import * as childProcess from "child_process";

function readFileAsync(filepath: string): Promise<Buffer> {
  return new Promise((resolve, reject) => {
    fs.readFile(filepath, (err, buffer) => {
      if (err) reject(err);
      else resolve(buffer);
    });
  });
}

function execAsync(command: string): Promise<[string, string]> {
  return new Promise((resolve, reject) => {
    childProcess.exec(command, (err, stdout, stderr) => {
      if (err) reject(err);
      else resolve([stdout, stderr]);
    });
  });
}

async function getRandomPath(): Promise<string> {
  return new Promise((resolve, reject) => {
    tmp.file((err, file) => {
      if (err) reject(err);
      else resolve(file);
    });
  });
}

function isVersionSmaller(target: string, candidate: string): boolean {
  const semVer1 = target.split(".");
  const semVer2 = candidate.split(".");
  for (let i = 0; i < semVer1.length; i++) {
    const component = semVer2[i];
    if (component && +component > +semVer1[i]) {
      return true;
    } else if (+component < +semVer1[i]) {
      return false;
    }
  }
  return false;
}

async function checkDependency(dependency: string): Promise<void> {
  const randomPath = await getRandomPath();
  const libName = dependency.split("/")[0];
  const version = dependency.split("/")[1];
  console.log(`-- ${dependency}`);
  await execAsync(`conan search ${libName} --json ${randomPath}`);
  const result = JSON.parse((await readFileAsync(randomPath)).toString());
  result.results[0].items.forEach((item: any) => {
    const recipeVersion = item.recipe.id.split("/")[1];
    if (isVersionSmaller(version, recipeVersion)) {
      console.log(`  => ${item.recipe.id}`);
    } else {
      console.log(`  -- ${item.recipe.id}`);
    }
  });
}

async function checkOutdatedDependencies() {
  console.log("Checking outdated dependencies...");

  const contents = (await readFileAsync("./dependencies.txt")).toString();
  const dependencies = contents.split("\n");
  for (let dependency of dependencies) {
    await checkDependency(dependency);
  }
}

async function main() {
  try {
    await checkOutdatedDependencies();
  } catch (err) {
    console.error(err.stack);
    process.exit(1);
  }
}

if (!module.parent) {
  main();
}
