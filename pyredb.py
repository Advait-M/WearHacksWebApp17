#!/usr/bin/env python3

from pyrebase import *

class ForgetMeNot:
    def __init__(self):
        self.config = {
            "apiKey": "AIzaSyASyA1nD_CMsjwefumq-8vUJzZxH6BOFNc",
            "authDomain": "wearhacks17-56091.firebaseapp.com",
            "databaseURL": "https://wearhacks17-56091.firebaseio.com",
            "storageBucket": "wearhacks17-56091.appspot.com"
            }
        self.firebase = initialize_app(self.config)

        self.db = self.firebase.database()


    def start(self):
        self.stream = self.db.stream(self.streamHandler)
        print(self.stream)
        # self.addSession("Jeffy", "Guffy", "2:00", "4:00", "Waterloo", "Medicare")
        # self.addSession("Ert", "Geh", "4:00", "9:00", "Yeth", "Medicare")
        # self.addSession("Bob", "Hiw", "1:00", "7:00", "Toronto", "Health Plus")
        #self.editSession("Nim", "Feteov", "15:00", "18:00", "Toronto", "med")
        #self.getAll()
        # times = {"Start Time" : "01:00", "End Time" : "03:00"}
        # self.db.child("Bob Fred").set(times)
        ##endTime = {"End Time": "8:00"}
        ##self.db.child("Bob Fred").set(endTime)
        
    def addSession(self, firstName, lastName, startTime, endTime, location, clinicName):
        self.db.child(firstName + " " + lastName).set({"Start Time" : startTime, "End Time" : endTime, "Location" : location, "Clinic Name" : clinicName})

    def addIndex(self, curIndex):
        self.db.child("Index").set({"Pos":curIndex})
        
    def editIndex(self, curIndex):
        self.db.child("Index").update({"Pos":curIndex})

    def addScript(self, script):
        self.db.child("Text").set({"Script":script})
    def editScript(self, script):
        self.db.child("Text").update({"Script":script})

    def alert(self, state, mssg):
        if state:
            self.db.child("Alerts").child("Alert").set({"state": "True", "mssg": mssg})
        else:
            self.db.child("Alerts").child("Alert").set({"state": "False", "mssg": mssg})
    def editSession(self, startTime, endTime, name, location, clinicName):
        if typeOfString == "Start Time" or typeOfString == "End Time":
            self.db.child(oldFirstName + " " + oldLastName).update({typeOfString : newString})

##    def getText(self):
##        # print((self.db.child("wait-no-more").get()).each())
##        # print(type(self.db))
##        # print((self.db.child("/").get()).each())
##        # for i in range(0, len(self.db)):
##        all_users = self.db.child("/").get()
##        masterList = []
##        for user in all_users.each():
##            text = (user.val())["curIndex"]
##        print(text)
##        return text

    def streamHandler(self, post):
        event = post["event"]
        key = post["path"]
        value = post["data"]

        if event == "put":
            print(key, ":", value)

if __name__ == "__main__":
    a = ForgetMeNot()
    a.start()
    #ForgetMeNot().getText()
    a.editIndex(0)
    #a.getText()
    a.addScript("I am seated in an office, surrounded by heads and bodies. My posture is consciously congruent to the shape of my hard chair. This is a cold room in University Administration, wood-walled, Remington-hung, double-windowed against the November heat, insulated from Administrative sounds by the reception area outside, at which Uncle Charles, Mr. deLint and I were lately received. I am in here. Three faces have resolved into place above summer-weight sportcoats and half-Windsors across a polished pine conference table shiny with the spidered light of an Arizona noon. These are three Deans — of Admissions, Academic Affairs, Athletic Affairs. I do not know which face belongs to whom. I believe I appear neutral, maybe even pleasant, though I've been coached to err on the side of neutrality and not attempt what would feel to me like a pleasant expression or smile. I have committed to crossing my legs I hope carefully, ankle on knee, hands together in the lap of my slacks. My fingers are mated into a mirrored series of what manifests, to me, as the letter X. The interview room's other personnel include: the University's Director of Composition, its varsity tennis coach, and Academy prorector Mr. A. deLint. C.T. is beside me; the others sit, stand and stand, respectively, at the periphery of my focus. The tennis coach jingles pocketchange. There is something vaguely digestive about the room's odor. The high-traction sole of my complimentary Nike sneaker runs parallel to the wobbling loafer of my mother's halfbrother, here in his capacity as Headmaster, sitting in the chair to what I hope is my immediate right, also facing Deans.")
    a.alert(False, "heee;sad")
