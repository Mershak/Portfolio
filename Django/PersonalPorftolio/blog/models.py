from django.db import models

class blog(models.Model):
    a=0
    title = models.CharField(max_length=50)
    pub_date = models.DateTimeField()
    body = models.TextField(default='')
    image = models.ImageField(upload_to='blogposts/',default='')

    def summary(self):
        return self.body[:100]

    def pub_date_pretty(self):
        return self.pub_date.strftime('%b %e %Y')

    def __str__(self):
        return self.title