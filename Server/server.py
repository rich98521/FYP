from datetime import date
import tornado.escape
import tornado.ioloop
import tornado.web
import mysql.connector

class VersionHandler(tornado.web.RequestHandler):
    def get(self):
        response = { 'version': '3.5.1',
                     'last_build':  date.today().isoformat() }
        self.write(response)
 
class GetGameByIdHandler(tornado.web.RequestHandler):
    def get(self, id):
        response = { 'id': int(id),
                     'name': 'Crazy Game',
                     'release_date': date.today().isoformat() }
        self.write(response)
 
class ServersHandler(tornado.web.RequestHandler):
    def get(self):
        cnx = mysql.connector.connect(user='root', password='98521', host='127.0.0.1', database='database1')
        cursor = cnx.cursor()
        query = ("SELECT * FROM Servers")

        #cursor.execute(query, (name))
        #data = cursor.fetchone()
        response = {}
        #for result in cursor.execute(query, multi = True):
        #	d[result[0]] = result[1]


        cursor.execute(query)
        results = cursor.fetchall()
        for res in results:
        	response[res[0]] = { 'count': res[1], 'ip': res[2] }


        cursor.close()
        cnx.close()
        self.write(response)

class UpdateServerHandler(tornado.web.RequestHandler):
    def post(self):
        self.set_header("Access-Control-Allow-Origin", "176.61.44.135")
        updateType = self.get_argument('type', '')
        count = self.get_argument('count', '')
        name = self.get_argument('name', '')
        ip = self.get_argument('ip', '')

        cnx = mysql.connector.connect(user='root', password='98521',
                              host='127.0.0.1',
                              database='database1')
        cursor = cnx.cursor()
        command = ""
        if updateType == 'update':
        	command = ("DELETE FROM Servers WHERE Name = '"+ name +"' OR ip = '"+ ip +"'; INSERT INTO Servers (name, count, ip) VALUES ('"+ name +"', "+ count +", '"+ ip +"')")
        elif updateType == 'delete':
        	command = ("DELETE FROM Servers WHERE Name = '"+ name +"'")

        for result in cursor.execute(command, multi = True):
            pass
        cnx.commit()
        cursor.close()
        cnx.close()

        response = { 'name': name, 'type': updateType, 'count': count }

        self.write(response)
 
application = tornado.web.Application([
    (r"/servers/", ServersHandler),
    (r"/updateserver/", UpdateServerHandler)
])
 
if __name__ == "__main__":
    application.listen(8888)
    tornado.ioloop.IOLoop.instance().start()