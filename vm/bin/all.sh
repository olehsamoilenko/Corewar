list=(
# 42.cor					#
# Asombra.cor				# OK
# Bazou.cor					# OK
# Car.cor					# very long
# Code_eater.cor			# OK
# Douceur_power.cor			# long
# Eraser.cor				# OK
# Explosive_Kitty.cor		# OK
# ForgottenOne.cor			# OK
# Gagnant.cor				# OK
# Hidden.cor				# OK
# Horace.cor				# OK
# Izi_Miaou.cor				# OK
# Kappa.cor					# OK
# Kitty_Miaou.cor			# OK
# Kittystrophic.cor			# OK
# Machine-gun.cor			# OK
# MarineKing.cor			# OK
# Misaka_Mikoto.cor			# OK
# NoIdea.cor				# Seg Fault
# O-Maley_Miaou.cor			# OK
# Octobre_Rouge_V4.2.cor	# long
# Rainbow_dash.cor			# OK
# Stargate0_1.cor			# OK
# THUNDER.cor				# OK
# TheHarvester.cor			# OK
# Varimathras.cor			# OK
# Wall.cor					# OK
# _.cor						# OK
# _honeybadger.cor			# OK
# barriere.cor				
# bee_gees.cor				
# big_feet.cor
bigzork.cor				# OK
# casimir.cor
# champ.cor
# corelol.cor
# darksasuke.cor
# doge.cor
# dubo.cor
# echo.cor
ex.cor					# OK
# fast.cor
fluttershy.cor			# OK
# gateau.cor
# gedeon.cor
# helltrain.cor
# jinx.cor
jumper.cor				# OK
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
mortel.cor				# OK
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
toto.cor					# OK
turtle.cor				# OK
# ultima.cor
# ultimate-surrender.cor
# wave.cor
# youforkmytralala.cor
zork.cor					# OK
)

for i in ${list[@]}; do
	echo "\t"$i
	sh bin/bool.sh resources/champs/cor/$i
done;
