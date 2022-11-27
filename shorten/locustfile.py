import time
from locust import HttpUser, task, between

class QuickstartUser(HttpUser):
    wait_time = between(1, 2.5)

    @task
    def hello_world(self):
        self.client.get("")

    @task
    def view_items(self):
        self.client.post ("", data={"url":"https://docs.influxdata.com/telegraf/v1.12/plugins/plugin-list/#exec"})

    @task
    def goto_url(self):
        self.client.get ('/QDgr')

    @task
    def stats(self):
        self.client.get ('/stats')
