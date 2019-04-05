list=(
# 42.cor					#
# Car.cor					# very long
# Douceur_power.cor			# long
# ForgottenOne.cor			# LONG
# Octobre_Rouge_V4.2.cor	# long
# helltrain.cor				# long
# barriere.cor				# big
# bee_gees.cor				# long
# big_feet.cor				#
# Asombra.cor				# OK
# Bazou.cor					# OK
# Code_eater.cor			# OK
# Eraser.cor				# OK
# Explosive_Kitty.cor		# OK
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
# NoIdea.cor				# OK
# O-Maley_Miaou.cor			# OK
# Rainbow_dash.cor			# OK
# Stargate0_1.cor			# OK
# THUNDER.cor				# OK
# TheHarvester.cor			# OK
# Varimathras.cor			# OK
# Wall.cor					# OK
# _.cor						# OK
# _honeybadger.cor			# OK
# bigzork.cor				# OK
# ex.cor					# OK
# fluttershy.cor			# OK
# jumper.cor				# OK
# mortel.cor				# OK
# toto.cor					# OK
# turtle.cor				# OK
# casimir.cor				# OK
# champ.cor					# OK
# corelol.cor				# OK
# darksasuke.cor			# OK
# doge.cor					# OK
# dubo.cor					# OK
# echo.cor					# OK
# fast.cor					# OK
# gateau.cor				# OK
# gedeon.cor				# OK
# zork.cor					# OK
jinx.cor
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
# ultima.cor
# ultimate-surrender.cor
# wave.cor
# youforkmytralala.cor
)

for i in ${list[@]}; do
	echo "\t"$i
	sh bin/bool.sh resources/champs/cor/$i
done;
