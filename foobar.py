from selenium import webdriver
import time
import urllib
fp = webdriver.FirefoxProfile()
fp.set_preference("browser.download.folderList",1)
fp.set_preference("browser.download.manager.showWhenStarting",False)
fp.set_preference("browser.helperApps.neverAsk.openFile","application/pdf,application/x-pdf")
fp.set_preference("browser.helperApps.neverAsk.saveToDisk","application/pdf,application/x-pdf")
fp.set_preference("pdfjs.disabled",True)
browser = webdriver.Firefox(firefox_profile=fp)
browser.get('http://foobar.iiitd.edu.in/contest/team/problems.php')
element = browser.find_element_by_name("login")
element.send_keys("9650516994")
password = browser.find_element_by_name("passwd")
password.send_keys("*Vinaya13")
submit = browser.find_element_by_xpath("//*[@type='submit']")
submit.click()
elements = []
browser.get('http://foobar.iiitd.edu.in/contest/team/index.php')
download = browser.find_elements_by_tag_name("th")
for i in download:
	string = i.get_attribute("title")
	if(string[:7]=="problem" and string[:15] != "problem 'BALLOT" and string[:31] != "problem 'auckFmbar and Product'" and string[:30] != "problem 'auckFmbar and Lights'" and string[:32] != "problem 'auckFmbar and Sequence'" and string[:31	] != "problem 'FOOBAR and MASTERCHEF'" and ):
		i.click()
		time.sleep(3)
