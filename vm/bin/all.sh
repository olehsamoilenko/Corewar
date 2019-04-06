list=(
# 42.cor					#
# Car.cor					# very long
# barriere.cor				# big
# ForgottenOne.cor			# LONG
# mandragore.cor			# LONG
# jinx.cor					# OK long
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
# justin_bee.cor			# OK
# kamelkaze.cor				# OK
# katchup.cor				# OK
# lapsang.cor				# OK
# le_pet_de_nonne.cor		# OK
# live.cor					# OK
# loose.cor					# OK
# lou_forktologue.cor		# OK
# littlepuppy.cor			# OK
# master_of_puppets.cor		# OK
# maxidef.cor				# OK
# salamahenagalabadoun.cor	# OK
# overwatch.cor				# OK
# pai_mu_tan.cor			# OK
# run_Kitty_RUN.cor			# OK
# sam_2.0.cor				# OK
# sencha.cor				# OK
# skynet.cor				# OK
# slider2.cor				# OK
# tchupka.cor				# OK
# terminator.cor			# OK
# ultima.cor				# OK
# ultimate-surrender.cor	# OK
# wave.cor					# 27438 OK
# meowluigi.cor				# 27438 OK
# mise_a_jour_windows95.cor	# 27438 OK
# Douceur_power.cor			# 27438 OK
# Octobre_Rouge_V4.2.cor	# 27438 OK
# helltrain.cor				# 27438 OK
# bee_gees.cor				# 27438 OK
# youforkmytralala.cor		# OK
)

for i in ${list[@]}; do
	echo "\t"$i
	sh bin/bool.sh resources/champs/cor/$i
done;
