ax = output.plot(y = 'Colony Size', color = 'b', legend = False, alpha = 0.75)
ax.axvspan(1, 31, facecolor='gray', alpha=0.3)
ax.axvspan(60, 90, facecolor='gray', alpha=0.3)
ax.axvspan(121, 151, facecolor='gray', alpha=0.3)
ax.axvspan(182, 212, facecolor='gray', alpha=0.3)
ax.axvspan(244, 273, facecolor='gray', alpha=0.3)
ax.axvspan(305, 334, facecolor='gray', alpha=0.3)
ax.axvspan(366, 396, facecolor='gray', alpha=0.3)
ax.axvspan(425, 455, facecolor='gray', alpha=0.3)
ax.axvspan(486, 516, facecolor='gray', alpha=0.3)
#
#
#for year in range(2002, 2011):
#    params["SimStart"] = "01/01/" + str(year)
#    params["SimEnd"] = "06/30/" + str(year + 1)
#    
#    vp = VarroaPop(parameters = params, weather_file = "VarroaPy/VarroaPy/files/weather/" + weatherFile + ".wth")
#    output = vp.run_model()
#    output = output.drop(output.index[0])
#    output['Inactive Foragers'] = output['Colony Size'] - output['Adult Drones'] -output['Adult Workers'] - output['Foragers']
#    
#    output.plot(y = variable, ax = ax, color = 'b', legend = False, alpha = 0.75)


#
y1 = output['Adult Workers']
y2 = output['Adult Drones']
y3 = output['Foragers']
y4 = output['Inactive Foragers']

x = output.index.values
y = np.vstack([y1, y2, y3, y4])
labels = ["Adult Workers", "Adult Drones", "Foragers", "Inactive Foragers"]   
ax.stackplot(x, y, labels = labels)
ax.legend(loc='upper left', fontsize = 'x-small')
ax.set_ylim(0, 140000)
plt.title('Colony Size (2090) vs Time')
plt.savefig('Plots/' + Location + '/OnePlus_Stacked_Colony_Size_2090.png', dpi = 1000)