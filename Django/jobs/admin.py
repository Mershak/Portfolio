from django.contrib import admin

from .models import Job #calls the job model

admin.site.register(Job) #adds the Job model to the admin page
