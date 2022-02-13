import argparse, os, shutil, subprocess, zipfile

FLAG_FILE = "Institute_Papyrus_Flags.flg"

def copy_vanilla_scripts(a_args):
	print("Copying vanilla scripts...")

	vanilla = (
		FLAG_FILE,
		"Action.psc",
		"Activator.psc",
		"Actor.psc",
		"ActorBase.psc",
		"ActorValue.psc",
		"Alias.psc",
		"Ammo.psc",
		"Armor.psc",
		"AssociationType.psc",
		"Book.psc",
		"CameraShot.psc",
		"Cell.psc",
		"Class.psc",
		"CombatStyle.psc",
		"CommonPropertiesScript.psc",
		"Component.psc",
		"Container.psc",
		"EncounterZone.psc",
		"Faction.psc",
		"FollowersScript.psc",
		"Form.psc",
		"FormList.psc",
		"Game.psc",
		"GlobalVariable.psc",
		"HeadPart.psc",
		"Idle.psc",
		"ImageSpaceModifier.psc",
		"ImpactDataSet.psc",
		"InputEnableLayer.psc",
		"Key.psc",
		"Keyword.psc",
		"LeveledItem.psc",
		"Location.psc",
		"LocationRefType.psc",
		"MagicEffect.psc",
		"Message.psc",
		"MiscObject.psc",
		"ObjectMod.psc",
		"ObjectReference.psc",
		"Outfit.psc",
		"Package.psc",
		"Perk.psc",
		"Projectile.psc",
		"Quest.psc",
		"Race.psc",
		"Scene.psc",
		"ScriptObject.psc",
		"Shout.psc",
		"Sound.psc",
		"Spell.psc",
		"Static.psc",
		"Terminal.psc",
		"TextureSet.psc",
		"Topic.psc",
		"TopicInfo.psc",
		"Utility.psc",
		"VisualEffect.psc",
		"VoiceType.psc",
		"Weapon.psc",
		"Weather.psc",
		"WordOfPower.psc",
		"WorldSpace.psc",
	)

	f4se = (
		"InstanceData.psc",
	)

	zipname = os.path.join(a_args.fo4_dir, "Data/Scripts/Source/Base", "Base.zip")
	with zipfile.ZipFile(zipname) as zip:
		for filename in vanilla:
			src = filename
			dst = os.path.join(a_args.bin_dir, "include", filename)
			with zip.open(src) as zf, open(dst, "wb") as f:
				shutil.copyfileobj(zf, f)

	zipname = os.path.join(a_args.fo4_dir, "Data/Scripts/Source/F4SE", "F4SE.zip")
	with zipfile.ZipFile(zipname) as zip:
		for filename in f4se:
			src = filename
			dst = os.path.join(a_args.bin_dir, "include", filename)
			with zip.open(src) as zf, open(dst, "wb") as f:
				shutil.copyfileobj(zf, f)

def compile_scripts(a_args):
	compiler = os.path.join(a_args.fo4_dir, "Papyrus Compiler", "PapyrusCompiler.exe")
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
	parser.add_argument("--fo4-dir", type=str, help="the fallout 4 directory", required=True)
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
