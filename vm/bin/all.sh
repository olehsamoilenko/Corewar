list=(
# 42.cor
Asombra.cor
# Bazou.cor
# Car.cor
# Code_eater.cor
# Douceur_power.cor
# Eraser.cor
# Explosive_Kitty.cor
# ForgottenOne.cor
Gagnant.cor
# Hidden.cor
# Horace.cor
# Izi_Miaou.cor
# Kappa.cor
# Kitty_Miaou.cor
# Kittystrophic.cor
# Machine-gun.cor
# MarineKing.cor
# Misaka_Mikoto.cor
# NoIdea.cor
# O-Maley_Miaou.cor
# Octobre_Rouge_V4.2.cor
# Rainbow_dash.cor
# Stargate0_1.cor
# THUNDER.cor
# TheHarvester.cor
# Varimathras.cor
# Wall.cor
# _.cor
# _honeybadger.cor
# barriere.cor
# bee_gees.cor
# big_feet.cor
bigzork.cor
# casimir.cor
# champ.cor
# corelol.cor
# darksasuke.cor
# doge.cor
# dubo.cor
# echo.cor
ex.cor
# fast.cor
fluttershy.cor
# gateau.cor
# gedeon.cor
# helltrain.cor
# jinx.cor
jumper.cor
# justin_bee.cor
# kamelkaze.cor
# katchup.cor
# lapsang.cor
# le_pet_de_nonne.cor
# littlepuppy.cor
# live.cor
# loose.cor
# lou_forktologue.cor
# mandragore.cor
# master_of_puppets.cor
# maxidef.cor
# meowluigi.cor
# mise_a_jour_windows95.cor
mortel.cor
# overwatch.cor
# pai_mu_tan.cor
# run_Kitty_RUN.cor
# salamahenagalabadoun.cor
# sam_2.0.cor
# sencha.cor
# skynet.cor
# slider2.cor
# tchupka.cor
# terminator.cor
toto.cor
turtle.cor
# ultima.cor
# ultimate-surrender.cor
# wave.cor
# youforkmytralala.cor
zork.cor
)

for i in ${list[@]}; do
	echo "\t"$i
	sh bin/bool.sh resources/champs/cor/$i
done
