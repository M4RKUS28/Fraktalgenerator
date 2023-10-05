function Component()
{
	// default constructor
}

Component.prototype.createOperations = function()
{
	// call default implementation to actually install Fraktalgenerator.exe
	component.createOperations();
	
	if(systemInfo.productType === "windows") {
		component.addOperation("CreateShortcut", "@TargetDir@/bin/Desktop/Windows/Fraktalgenerator.exe",
							"@StartMenuDir@/Fraktalgenerator.lnk",
							"description=A programm to calculate the mandelbrot-set and julia-set.");
		component.addOperation("CreateShortcut", "@TargetDir@/bin/Desktop/Windows/Fraktalgenerator.exe",
							"@HomeDir@/Desktop/Fraktalgenerator.lnk",
							"description=A programm to calculate the mandelbrot-set and julia-set.");					
	
	}


}