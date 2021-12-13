import argparse, os, shutil, subprocess, zipfile

FLAG_FILE = "TESV_Papyrus_Flags.flg"

def copy_vanilla_scripts(a_args):
	print("Copying vanilla scripts...")

	vanilla = (
		FLAG_FILE,
		"Actor.psc",
		"ActorBase.psc",
		"Alias.psc",
		"Ammo.psc",
		"Armor.psc",
		"AssociationType.psc",
		"Cell.psc",
		"Class.psc",
		"EncounterZone.psc",
		"Faction.psc",
		"Form.psc",
		"FormList.psc",
		"Game.psc",
		"GlobalVariable.psc",
		"Idle.psc",
		"ImageSpaceModifier.psc",
		"ImpactDataSet.psc",
		"Key.psc",
		"Keyword.psc",
		"Location.psc",
		"LocationRefType.psc",
		"MagicEffect.psc",
		"MiscObject.psc",
		"ObjectReference.psc",
		"Outfit.psc",
		"Package.psc",
		"Perk.psc",
		"Projectile.psc",
		"Quest.psc",
		"Race.psc",
		"Scene.psc",
		"Shout.psc",
		"Spell.psc",
		"TextureSet.psc",
		"Topic.psc",
		"Utility.psc",
		"VoiceType.psc",
		"Weapon.psc",
		"WordOfPower.psc",
		"WorldSpace.psc",
	)

	zipname = os.path.join(a_args.sse_dir, "Data", "Scripts.zip")
	with zipfile.ZipFile(zipname) as zip:
		for filename in vanilla:
			src = "Source/Scripts/{}".format(filename)
			dst = os.path.join(a_args.bin_dir, "include", filename)
			with zip.open(src) as zf, open(dst, "wb") as f:
				shutil.copyfileobj(zf, f)

def compile_scripts(a_args):
	compiler = os.path.join(a_args.sse_dir, "Papyrus Compiler", "PapyrusCompiler.exe")
	vanilla = os.path.join(a_args.bin_dir, "include")
	src = os.path.join(a_args.src_dir, "src", "papyrus")
	output = os.path.join(a_args.bin_dir, "bin")

	subprocess.run([
			compiler,
			src,
			"-import={};{}".format(src, vanilla),
			"-output={}".format(output),
			"-flags={}".format(FLAG_FILE),
			"-all",
		],
		check=True)

def parse_arguments():
	parser = argparse.ArgumentParser(description="compile papyrus source scripts")
	parser.add_argument("--bin-dir", type=str, help="the project binary directory", required=True)
	parser.add_argument("--sse-dir", type=str, help="the skyrim special edition directory", required=True)
	parser.add_argument("--src-dir", type=str, help="the project root source directory", required=True)
	parser.add_argument("--target", type=str, help="the target to execute", required=True)
	return parser.parse_args()

def main():
	args = parse_arguments()

	os.makedirs(os.path.join(args.bin_dir, "include"), exist_ok=True)
	os.makedirs(os.path.join(args.bin_dir, "bin"), exist_ok=True)

	targets = {
		"copy_vanilla": copy_vanilla_scripts,
		"compile": compile_scripts,
	}

	targets[args.target](args)

if __name__ == "__main__":
	main()
